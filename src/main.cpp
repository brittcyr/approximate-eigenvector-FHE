#include <cmath>
#include <NTL/ZZ_p.h>
#include "FHE.h"

using namespace NTL;
using namespace std;

int main()  {
  setTimersOn();

  // Result should be 1000x1000 with q = 2**30
  ZZ q;
  q = 1 << 16;
  long n = 16;
  long m = n * std::floor(log2(to_long(q)));
  
  ZZ_p::init(q);
  // Generate secret key
  FHESecKey sk = FHESecKey(n);
  // Generate public key
  FHEPubKey pk = FHEPubKey(m, n, sk);

  ZZ_p o = conv<ZZ_p>(8);
  ZZ_p z = conv<ZZ_p>(2);
  Ctxt one = pk.Encrypt(o);
  Ctxt zero = pk.Encrypt(z);

  ZZ_p plain0 = sk.MPDec(one);
  // Test decrypting before homomorphism
  cout << "Decrypts to " << plain0 << "\n";

  // Test Multiply by Constant
  one.multByConstant(conv<ZZ_p>(10));
  ZZ_p plain = sk.MPDec(one);
  cout << "Decrypts to " << plain << "\n";

  // Test Addition
  one.addCtxt(zero);
  plain = sk.MPDec(one);
  cout << "Decrypts to " << plain << "\n";

  // Test Multiplication
  one.multiplyBy(zero);
  plain = sk.MPDec(one);
  cout << "Decrypts to " << plain << "\n";
  printAllTimers();
}
