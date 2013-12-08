#include "FHE.h"
#include "matrix_ops.h"
#include "timing.h"
#include <NTL/matrix.h>
#include <NTL/ZZ_p.h>
#include <NTL/mat_ZZ_p.h>

NTL_CLIENT
using namespace NTL;

/******************** FHESecKey implementation **********************/
/********************************************************************/
FHESecKey::FHESecKey(long n) {
  t.SetLength(n);
  long i, j;

  // Fill t with random integers
  for (i = 0; i < n; i++) {
    t[i] = random_ZZ_p();
  }

  // Set sk = [1 -t[0] -t[1] ...]
  sk.SetLength(n + 1);
  sk[0] = 1;
  for (i = 1; i <= n; i++) {
    sk[i] = -t[i - 1];
  }

  long l = std::floor(log2(to_long(ZZ_p::modulus()))) + 1;
  long N = l*(n + 1);

  // Fill v with PowersOfTwo(sk)
  v.SetLength(N);
  for (i = 0; i < (n+1); i++) {
    for (j = 0; j < l; j++) {
      v[l * i + j] = sk[i] * (1<<j);
    }
  }
}

// Decryption
ZZ_p FHESecKey::Decrypt(const Ctxt &ciphertxt) const
{
  FHE_TIMER_START;
  
  Mat<ZZ_p> mat = ciphertxt.A;
  long q = to_long(ZZ_p::modulus());
  long l = std::floor(log2(q)) + 1;

  // Find i to be the correct row;
  long i = 0;
  for (i = 0; i < l; i++) {
    if (1 << i > q >> 2) {
      break;
    }
  }
  
  Vec<ZZ_p> row = mat[i];
  ZZ_p x = row*v;
  long x_i = conv<long>(x);
  ZZ_p result_bit = conv<ZZ_p>(((x_i + (1 << (i-1))) / (1 << i))%2);
  FHE_TIMER_STOP;
  return result_bit;
}

// MPDecryption
ZZ_p FHESecKey::MPDec(const Ctxt &ciphertxt) const
{
  FHE_TIMER_START;
  
  long q = to_long(ZZ_p::modulus());
  long l = std::floor(log2(q)) + 1;
  Mat<ZZ_p> C = ciphertxt.A;

  bool bit;
  ZZ_p x, result = conv<ZZ_p>(0);
  long x_long;
  for (long i = l-2; i >= 0; i--) {
    x = C[i]*v;
    // Remove all the result so it is just the current bit
    x -= conv<ZZ_p>(result * (1 << i));
    x_long = conv<long>(x);
    // Need to determine if x is small or not relative to q
    bit = min(x_long, q-x_long) >= std::abs(x_long - (q/2));
    result += conv<ZZ_p>(bit << (l-2 - i));
  }
  FHE_TIMER_STOP;
  return result;
}

ostream& operator<<(ostream& str, const FHESecKey& p)
{
  return str << "secret key " << p.sk << "\n"; 
}

/******************** FHEPubKey implementation **********************/
/********************************************************************/
FHEPubKey::FHEPubKey(long m, long n, FHESecKey& sk) {
  Mat<ZZ_p> B;
  B.SetDims(m, n);

  // Fill B with random integers
  long i,j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      B[i][j] = random_ZZ_p();
    }
  }

  // Create the error vector
  Vec<ZZ_p> e;
  long stdev = 8;
  e.SetLength(m);
  for (i = 0; i < m; i++) {
    static double const Pi=4.0*atan(1.0); // Pi=3.1415..
    static long const bignum = 0xfffffff;

    // Uses the Box-Muller method to get two Normal(0,stdev^2) variables
    double r1 = (1+RandomBnd(bignum))/((double)bignum+1);
    double r2 = (1+RandomBnd(bignum))/((double)bignum+1);
    double theta=2*Pi*r1;
    double rr= sqrt(-2.0*log(r2))*stdev;

    // Generate two Gaussians RV's, rounded to integers
    long x = (long) floor(rr*cos(theta) +0.5);
    e[i] = x;
  }

  // Get vector t from sk
  Vec<ZZ_p> t = sk.t;

  A.SetDims(m, n + 1);
  // Fill the first column of A with b
  // Create b by multiplying B and t and adding error
  for (i = 0; i < m; i++) {
    long actual_sum = 0;
    for (j = 0; j < n; j++) {
      actual_sum += conv<long>(B[i][j] * t[j]);
    }
    A[i][0] = actual_sum + conv<long>(e[i]);
  }

  // Fill the rest of A with B
  for (i = 0; i < m; i++) {
    for (j = 1; j <= n; j++) {
      A[i][j] = B[i][j - 1];
    }
  }
}

// Encryption
Ctxt FHEPubKey::Encrypt(ZZ_p& ptxt) const
{
  FHE_TIMER_START;

  long l = std::floor(log2(to_long(ZZ_p::modulus()))) + 1;
  long n = A.NumCols() - 1;
  long m = A.NumRows();
  long N = (n+1) * l;

  // Build a random matrix
  Mat<ZZ_p> R;
  R.SetDims(N, m);

  long i,j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < m; j++) {
      R[i][j] = RandomBnd(2);
    }
  }

  Mat<ZZ_p> ident = ident_mat_ZZ_p(N);
  ident *= ptxt;

  Mat<ZZ_p> cipher = Flatten(ident + BitDecomp(R*A));
  Ctxt ctxt(cipher);

  FHE_TIMER_STOP;
  return ctxt;
}

ostream& operator<<(ostream& str, const FHEPubKey& p)
{
  return str << "public key " << p.A << "\n"; 
}
