#include <NTL/xdouble.h>
#include <NTL/ZZ_p.h>
#include <NTL/matrix.h>
#include "timing.h"
#include <iostream>

using namespace NTL;
using namespace std;

class FHEPubKey;
class FHESecKey;

class Ctxt {
public:
  Mat<ZZ_p> A; // Ciphertext matrix

  Ctxt(); // constructor
  Ctxt(Mat<ZZ_p> mat); // constructor
  Ctxt& operator=(const Ctxt& other);  // assignment

  bool operator==(const Ctxt& other) const { return *this == other;}
  bool operator!=(const Ctxt& other) const { return *this != other;}

  void clear() {
    A.kill();
  }

  // Encryption and decryption are done by FHE[Pub|Sec]Key

  // Add ciphertexts
  Ctxt& operator+=(const Ctxt& other) { addCtxt(other); return *this; }
  void addCtxt(const Ctxt& other);

  // Ciphertext multiplication
  Ctxt& operator*=(const Ctxt& other) { multiplyBy(other); return *this; }
  void multiplyBy(const Ctxt& other);

  // Multiply-by-constant. If the size is not given, we use
  // phi(m)*ptxtSpace^2 as the default value.
  void multByConstant(const ZZ_p& alpha);

};
ostream& operator<<(ostream& str, const Ctxt& c);
