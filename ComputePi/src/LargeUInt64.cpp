#include "..\lib\LargeUInt64.h"

LargeDecimal::LargeDecimal(string s)
{
	ndigits = (int)s.length();
	digits = new char[ndigits];
	for (int i = 0; i < ndigits; i++)
	{
		digits[i] = s[ndigits - i - 1] - '0';
	}

}

LargeDecimal::~LargeDecimal()
{
	ndigits = 0;
	delete[] digits;
}

string LargeDecimal::tostr()
{
	string s(ndigits, 0);
	for (int i = 0; i < ndigits; i++)
	{
		s[i] = digits[ndigits - i - 1] + '0';
	}
	return s;
}

LargeUInt64::LargeUInt64(LargeUInt64 &lui64)
{
	ndigits = lui64.ndigits;
	sign = lui64.sign;
	storelen = lui64.ndigits;
	digits = new uint64_t[ndigits];
	memcpy(digits, lui64.digits, sizeof(uint64_t)*ndigits);
}

LargeUInt64::LargeUInt64(void) : ndigits(0), sign(1), storelen(0)
{
	digits = NULL;
}

LargeUInt64::LargeUInt64(int n) : ndigits(n), sign(1), storelen(n)
{
	digits = new uint64_t[n];
	//_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	memset(digits, 0, sizeof(uint64_t)*n);
}

LargeUInt64::LargeUInt64(LargeDecimal &dec) : sign(1)
{
	ConvfromDec(dec);
}

LargeUInt64::LargeUInt64(uint64_t data[], int length) : sign(1)
{
	ndigits = length;
	storelen = length;
	digits = new uint64_t[length];
	memcpy(digits, data, sizeof(uint64_t)*length);
}

LargeUInt64::LargeUInt64(uint64_t data[], int length, int padding) : sign(1)
{
	int lenpad = length + padding;
	ndigits = lenpad;
	storelen = lenpad;
	digits = new uint64_t[lenpad];

	memset(digits, 0, padding * sizeof(uint64_t));
	memcpy(digits + padding, data, length * sizeof(uint64_t));
}

LargeUInt64::LargeUInt64(uint64_t data) : sign(1)
{

	ndigits = 1;
	storelen = 1;
	digits = new uint64_t[1];
	digits[0] = data;
}

LargeUInt64::~LargeUInt64()
{
	if (digits != NULL)
	{
		delete[] digits;
		digits = NULL;
		ndigits = 0;
		storelen = 0;
	}
}


LargeUInt64& LargeUInt64::operator= (const LargeUInt64 &i)
{
	if (digits != NULL)
		delete[] digits;
	ndigits = i.ndigits;
	sign = i.sign;
	storelen = i.ndigits;
	digits = new uint64_t[ndigits];
	memcpy(digits, i.digits, sizeof(uint64_t)*ndigits);
	return *this;
}

string LargeUInt64::Conv2Dec()
{
	int base = 1000000000;
	uint64_t remder = 0;

	uint32_t* dem = new uint32_t[2 * ndigits];
	memcpy(dem, digits, ndigits * sizeof(uint64_t));
	int n = 2 * ndigits;

	uint64_t* dectmp = new uint64_t[(int)(1.071 * n + 0.5)];
	int len_dec = 0;
	do{
		for (int i = (n - 1); i >= 0; i--)
		{
			uint64_t u = dem[i] + (remder << 32);
			lldiv_t tmp = lldiv(u, base);
			dem[i] = (uint32_t)tmp.quot;
			remder = tmp.rem;
		}
		while (dem[n - 1] == 0 && n > 0)
			n--;
		dectmp[len_dec++] = remder;
		remder = 0;
	} while (n > 0);

	int len = len_dec * 9;

	char* digit = new char[len + 1];
	int j = 0;
	for (int i = len_dec - 1 ; i >= 0; i--)
	{
		uint64_t tmp = dectmp[i];
		sprintf_s(digit + (j++ * 9), 11, "%09llu", tmp);
	}

	j = 0;
	while (digit[j] == '0')
		j++;
	digit[len] = 0;
	string s(digit + j);


	return s;
}

void LargeUInt64::ConvfromDec(LargeDecimal &dec)
{
	int n = dec.ndigits;
	LargeUInt64 a = ConvfromDecRecu(dec, 0, n);
	while (a.digits[a.ndigits - 1] == 0)
		a.ndigits--;
	ndigits = a.ndigits;
	storelen = a.ndigits;
	digits = new uint64_t[a.ndigits];
	memcpy(digits, a.digits, sizeof(uint64_t)*a.ndigits);

}

LargeUInt64 LargeUInt64::ConvfromDecRecu(LargeDecimal &dec, int left, int right) // left close, right open
{
	int len = right - left;
	if (len == 1)
	{
		LargeUInt64 result(len);
		for (int i = left; i < right; i++)
		{
			result.digits[i - left] = dec.digits[i];
		}
		return result;
	}

	int mid = (left + right) / 2;
	LargeUInt64 low = ConvfromDecRecu(dec, left, mid);
	LargeUInt64 high = ConvfromDecRecu(dec, mid, right);

	LargeUInt64 tmp = power(10, len / 2);
	LargeUInt64 tmp2 = high * tmp;
	LargeUInt64 result = tmp2 + low;


	return result;
}

LargeUInt64 power(uint64_t base, int exp)
{
	LargeUInt64 a(exp + 1);
	memset(a.digits, 0, sizeof(uint64_t)* a.storelen);
	a.digits[0] = 1;
	a.ndigits = 1;
	LargeUInt64 b(1);
	b.digits[0] = base;
	for (int i = 0; i < exp; i++)
	{
		a = a * b;
	}

	return a;
}

LargeUInt64 operator+ (const LargeUInt64 &i1, const LargeUInt64 &i2)
{
	int max = i1.ndigits;
	int min = i2.ndigits;
	if (max < min)
	{
		return (i2 + i1);
	}

	LargeUInt64 a(max + 1);
	a.ndigits = max + 1;
	uint64_t* pa = a.digits;
	unsigned char carry = 0;
	for (int i = 0; i < min; i++)
	{
		carry = _addcarry_u64(carry, i1.digits[i], i2.digits[i], pa + i);
	}
	for (int i = min; i < max; i++)
	{
		if (carry == 0)
		{
			memcpy(pa + i, i1.digits + i, sizeof(uint64_t)*(max - i));
			break;
		}
		carry = _addcarry_u64(carry, i1.digits[i], 0, pa + i);
	}
	pa[max] = carry;
	while (pa[a.ndigits - 1] == 0 && a.ndigits > 1)
		a.ndigits--;

	return a; 
}


LargeUInt64 operator- (const LargeUInt64 &i1, const LargeUInt64 &i2)
{
	int n1 = i1.ndigits;
	int n2 = i2.ndigits;
	if (n1 > n2)
	{
		LargeUInt64 a = minusp(i1, i2, n1, n2);
		a.sign = 1;
		return a;		
	}
	else if (n1 < n2)
	{
		LargeUInt64 a = minusp(i2, i1, n2, n1);
		a.sign = -1;
		return a;
	}
	else
	{
		for (int i = n1 - 1; i >= 0; i--)
		{
			if (i1.digits[i] > i2.digits[i])
			{
				LargeUInt64 a = minusp(i1, i2, i + 1, i + 1);
				a.sign = 1;
				return a;
			}
			if (i1.digits[i] < i2.digits[i])
			{
				LargeUInt64 a = minusp(i2, i1, i + 1, i + 1);
				a.sign = -1;
				return a;
			}
		}
		LargeUInt64 a(0);
		return a;
	}


}

LargeUInt64 minusp(const LargeUInt64 &i1, const LargeUInt64 &i2, int len1, int len2) // len1 >= len2, i1 >= i2
{
		unsigned char borrow = 0;
		LargeUInt64 a(len1);
		uint64_t* pa = a.digits;
		for (int i = 0; i < len2; i++)
		{
			borrow = _subborrow_u64(borrow, i1.digits[i], i2.digits[i], pa + i);
		}
		for (int i = len2; i < len1; i++)
		{
			if (borrow == 0)
			{
				memcpy(pa + i, i1.digits + i, sizeof(uint64_t)*(len1 - i));
				break;
			}
			borrow = _subborrow_u64(borrow, i1.digits[i], 0, pa + i);
		}
		while (a.digits[a.ndigits - 1] == 0 && a.ndigits > 1)
			a.ndigits--;
	
		return a;
}

//void LargeUInt64::plusloc(int loc, uint64_t num)
//{
//	unsigned char carry = 0;
//	carry = _addcarry_u64(carry, digits[loc], num, digits + loc);
//
//	carry = _addcarry_u64(carry, digits[loc + 1], 0, digits +1+ loc);
//
//}

//extern FFT fftbuffer;
LargeUInt64 lowermulti (const LargeUInt64 &i1, const LargeUInt64 &i2)
{
//	return MulWithFFT(fftbuffer, i1, i2);
	int len1 = i1.ndigits;
	int len2 = i2.ndigits;
	int length = len1 + len2;
	LargeUInt64 tmp(length);
	memset(tmp.digits, 0, sizeof(uint64_t)* length);

	MultiUInt(i1.digits, len1, i2.digits, len2, tmp.digits);

	while (tmp.digits[length - 1] == 0 && length > 1)
		length--;
	tmp.ndigits = length;
	tmp.sign = i1.sign * i2.sign;
	return tmp;

}


LargeUInt64 operator*(const LargeUInt64 &i1, const LargeUInt64 &i2)
{
	//return lowermulti(i1, i2);
	int len1 = i1.ndigits;
	int len2 = i2.ndigits;

	int threshold = 100;
	if (len1 <= threshold || len2 <= threshold)
		return lowermulti(i1, i2);

	int max = len1 > len2 ? len1 : len2;
	int min = len1 < len2 ? len1 : len2;
	int m = max / 2;
	if (m > min)
		m = min;

	LargeUInt64 x[3];
	LargeUInt64 y[3];
	LargeUInt64 z[3];

	x[1] = LargeUInt64(i1.digits + m, len1 - m);
	x[0] = LargeUInt64(i1.digits, m);
	y[1] = LargeUInt64(i2.digits + m, len2 - m);
	y[0] = LargeUInt64(i2.digits, m);
	x[2] = x[0] + x[1];
	y[2] = y[0] + y[1];

	if (m > 400)
	{
		Concurrency::parallel_for(size_t(0), size_t(3), [&](size_t i)
		{
			z[i] = x[i] * y[i];
		});
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			z[i] = x[i] * y[i];
		}
	}


	z[2] = z[2] - z[0] - z[1];

	LargeUInt64 finalz2(z[2].digits, z[2].ndigits, m);

	LargeUInt64 finalz1(z[1].digits, z[1].ndigits, 2 * m);

	LargeUInt64 tmp = finalz1 + finalz2 + z[0];
	int length = tmp.ndigits;
	while (tmp.digits[length - 1] == 0 && length > 1)
		length--;
	tmp.ndigits = length;
	tmp.sign = i1.sign * i2.sign;
	return tmp;
}

void MultiUInt(uint64_t* p1, int len1, uint64_t* p2, int len2, uint64_t* ans)
{

	uint64_t high;
	uint64_t low;
	unsigned char carry = 0;
	for (int i = 0; i < len1; i++)
	{
		uint64_t i1t = p1[i];
		uint64_t* tmp1 = ans + i;
		if (i1t)
		{
			for (int j = 0; j < len2; j++)
			{
				uint64_t i2t = p2[j];
				if (i2t)
				{
					low = _umul128(i1t, i2t, &high);
					uint64_t* ptmp = tmp1 + j;
					carry = _addcarry_u64(0, *ptmp, low, ptmp);
					carry = _addcarry_u64(carry, *(ptmp + 1), high, ptmp + 1);
					carry = _addcarry_u64(carry, *(ptmp + 2), 0, ptmp + 2);
				}
			}
		}
	}
}
//
//LargeUInt64 LargeUInt64::operator *= (const LargeUInt64 &lui)
//{
//	int len1 = ndigits;
//	int len2 = lui.ndigits;
//	ndigits += len2;
//
//	uint64_t *high = new uint64_t[len1 * len2];
//	uint64_t *low = new uint64_t[len1 * len2];
//	for (int i = 0; i < len1; i++)
//	{
//		for (int j = 0; j < len2; j++)
//		{
//			low[i * len2 + j] = _umul128(digits[i], lui.digits[j], high + i * len2 + j);
//		}
//	}
//	memset(digits, 0, sizeof(uint64_t)* ndigits);
//	unsigned char carry;
//	for (int i = 0; i < len1; i++)
//	{
//		for (int j = 0; j < len2; j++)
//		{
//			/*plusloc(i + j, low[i * len2 + j]);
//			plusloc(i + j + 1, high[i * len2 + j]);*/
//			int t = i + j;
//			carry = 0;
//			carry = _addcarry_u64(carry, digits[t], low[i * len2 + j], digits + t);
//			carry = _addcarry_u64(carry, digits[t + 1], high[i * len2 + j], digits + t + 1);
//			carry = _addcarry_u64(carry, digits[t + 2], 0, digits + t + 2);
//		}
//	}
//	delete[] low;
//	delete[] high;
//	while (digits[ndigits - 1] == 0 && ndigits > 1)
//		ndigits--;
//	
//	return *this;
//}
//
//
//LargeUInt64 MulWithFFT(FFT &fftbuffer, const LargeUInt64 &A, const LargeUInt64 &B)
//{
//	uint32_t* ACoef = (uint32_t*)A.digits;
//	int ASize = A.ndigits * 2;
//	uint32_t* BCoef = (uint32_t*)B.digits;
//	int BSize = B.ndigits * 2;
//	int CSize = ASize + BSize - 1;
//	double* CCoef = new double[CSize];
//	fftbuffer.lowerMulWithFFT(ACoef, ASize, BCoef, BSize, CCoef);
//
//	int length = A.ndigits + B.ndigits;
//	LargeUInt64 tmp(length);
//	memset(tmp.digits, 0, sizeof(uint64_t)* length);
//	uint32_t* tmpdigit = (uint32_t*)tmp.digits;
//	for (int i = 0; i < CSize; i++)
//	{
//		double d = CCoef[i];
//		uint32_t a[10];
//		int j = 0;
//		uint32_t tmpa;
//		do
//		{
//			tmpa = (uint32_t)(d );
//			a[j++] = tmpa;
//			d /= 0x100000000;
//		} while (tmpa != 0);
//		unsigned char carry = 0;
//		for (int k = 0; k < j; k++)
//		{
//			carry = _addcarry_u32(carry, a[k], tmpdigit[i + k], tmpdigit + i + k);
//		}
//		_addcarry_u32(carry, 0, tmpdigit[i + j], tmpdigit + i + j);
//	}
//	while (tmp.digits[length - 1] == 0 && length > 1)
//		length--;
//	tmp.ndigits = length;
//	delete[] CCoef;
//	return tmp;
//}