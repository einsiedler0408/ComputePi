#ifndef ComputePi_BigFloat_H_
#define ComputePi_BigFloat_H_

#include "Global.h"
#include "LargeUInt64.h"

class BigFloat
{
public:
	int exp;
	int sign;
	int MulExactNum;
	LargeUInt64 mantissa;

	BigFloat(void);
	BigFloat(double d, int mulexactnum);
	BigFloat(BigFloat &bigfloat, int mulexactnum);
	BigFloat(LargeUInt64 largeuint, int mulexactnum);
	BigFloat(uint64_t data[], int length, int mulexactnum);
	BigFloat(BigFloat &i);
	~BigFloat(void);

	BigFloat& BigFloat::operator= (const BigFloat &i);
	string ftostr(int n);
	void Display(int n);
	friend BigFloat operator+ (const BigFloat &i1, const BigFloat &i2);
	friend BigFloat operator- (const BigFloat &i1, const BigFloat &i2);
	friend BigFloat operator* ( BigFloat &i1,  BigFloat &i2);

	friend BigFloat Reciprocal(BigFloat denom, int bits);
	friend BigFloat InverseSquareRoot(double C, int bits);

private:

};



#endif //ComputePi_BigFloat_H_