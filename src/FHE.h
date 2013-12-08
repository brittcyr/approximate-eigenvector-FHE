#include <vector>
#include "Ctxt.h"
#include <NTL/ZZ_p.h>
#include <NTL/matrix.h>
#include "timing.h"
#include <iostream>

using namespace NTL;
using namespace std;

/**
 * @class FHESecKey
 * @brief The secret key
******************************************************************/
class FHESecKey { // The secret key
public:
  Vec<ZZ_p> sk; // sk = [1 -t[0] -t[1] ... -t[-1]]
  Vec<ZZ_p> t; // t = random length n vector from Z_q
  Vec<ZZ_p> v; // v = PowersOfTwo(s)
  FHESecKey(long n); // q is already set

  bool operator==(const FHESecKey& other) const {return *this==other;}
  bool operator!=(const FHESecKey& other) const {return !(*this==other);}

  // Decryption
  ZZ_p Decrypt(const Ctxt &ciphertxt) const;
  ZZ_p MPDec(const Ctxt &ciphertxt) const;
};
ostream& operator<<(ostream& str, const FHESecKey& p);

/**
 * @class FHEPubKey
 * @brief The public key
 ********************************************************************/
class FHEPubKey { // The public key

public:
  Mat<ZZ_p> A; // Matrix of the public key
  FHEPubKey(long m, long n, FHESecKey& sk);

  bool operator==(const FHEPubKey& other) const {return *this==other;}
  bool operator!=(const FHEPubKey& other) const {return !(*this==other);}

  // Encrypt
  Ctxt Encrypt(ZZ_p& plaintxt) const;
};
ostream& operator<<(ostream& str, const FHEPubKey& p);
