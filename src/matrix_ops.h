#include <vector>
#include <NTL/ZZ_p.h>
#include <NTL/matrix.h>

using namespace NTL;

Mat<ZZ_p> BitDecomp(Mat<ZZ_p> old);
Vec<ZZ_p> BitDecomp(Vec<ZZ_p> old);
Mat<ZZ_p> BitDecompInv(Mat<ZZ_p> old);
Vec<ZZ_p> BitDecompInv(Vec<ZZ_p> old);
Mat<ZZ_p> Flatten(Mat<ZZ_p> old);
