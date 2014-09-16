#ifndef ComputePi_LargeUInt64_H_
#define ComputePi_LargeUInt64_H_

#include "Global.h"

class LargeDecimal
{
public:
	char* digits;
	int ndigits;

	LargeDecimal(string s);
	
	~LargeDecimal(void);

	string tostr(void);

};


class LargeUInt64
{
public:
	uint64_t*  digits;
	int ndigits;
	int storelen;
	char sign; // 1 for +, -1 for -

	LargeUInt64(void);
	LargeUInt64(LargeUInt64 &lui64);
	LargeUInt64(int ndigits);
	LargeUInt64(LargeDecimal &dec);
	LargeUInt64(uint64_t data);
	LargeUInt64(uint64_t data[], int length);
	LargeUInt64(uint64_t data[], int length, int padding);
	~LargeUInt64(void);

	string Conv2Dec(void);
	void ConvfromDec(LargeDecimal &dec);

	LargeUInt64& operator= (const LargeUInt64 &i);

	friend LargeUInt64 operator+ (const LargeUInt64 &i1, const LargeUInt64 &i2);
	friend LargeUInt64 operator- (const LargeUInt64 &i1, const LargeUInt64 &i2);
	friend LargeUInt64 operator* (const LargeUInt64 &i1, const LargeUInt64 &i2);
	//friend LargeUInt64 MulWithFFT(FFT &fftbuffer, const LargeUInt64 &A, const LargeUInt64 &B);
	friend LargeUInt64 power(uint64_t base, int exp);

private:
	//void plusloc(int loc, uint64_t num);
	friend LargeUInt64 minusp(const LargeUInt64 &i1, const LargeUInt64 &i2, int len1, int len2);
	LargeUInt64 ConvfromDecRecu(LargeDecimal &dec, int left, int right);
	friend void MultiUInt(uint64_t* p1, int len1, uint64_t* p2, int len2, uint64_t* ans);
	friend LargeUInt64 lowermulti(const LargeUInt64 &i1, const LargeUInt64 &i2);
	//LargeUInt64 operator*= (const LargeUInt64 &i);
};



#endif //ComputePi_LargeUInt64_H_