#include "Ctxt.h"
#include "matrix_ops.h"
#include "timing.h"
#include <NTL/mat_ZZ_p.h>

NTL_CLIENT

Ctxt::Ctxt(Mat<ZZ_p> mat) {
  A = mat;
}

// Assignment
Ctxt& Ctxt::operator=(const Ctxt& other)
{
  A = other.A;
  return *this;
}

/********************************************************************/
// Ciphertext arithmetic

// Add another ciphertxt
void Ctxt::addCtxt(const Ctxt& other)
{
  FHE_TIMER_START;
  // A = Flatten(A + other.A);
  A += Flatten(other.A);
  FHE_TIMER_STOP;
}

// Multiply another ciphertext
void Ctxt::multiplyBy(const Ctxt& other)
{
  FHE_TIMER_START;
  // A = Flatten(A * other.A);
  A = Flatten(A * other.A);
  FHE_TIMER_STOP;
}

// Multiply another ciphertext
void Ctxt::multByConstant(const ZZ_p& alpha)
{
  FHE_TIMER_START;
  // A = Flatten(A * other.A);
  Mat<ZZ_p> M_alpha = Flatten(ident_mat_ZZ_p(A.NumRows()) * alpha);
  A = Flatten(A * M_alpha);
  FHE_TIMER_STOP;
}

/********************************************************************/
// Utility methods

ostream& operator<<(ostream& str, const Ctxt& c)
{
  return str << "ciphertext \n" << c.A;
}
