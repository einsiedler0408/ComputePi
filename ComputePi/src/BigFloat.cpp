#include "..\lib\BigFloat.h"

BigFloat::BigFloat(void)
{

}

BigFloat::BigFloat(LargeUInt64 largeuint, int mulexactnum) : sign(1), MulExactNum(mulexactnum)
{
	int j1 = 0;
	while (largeuint.digits[j1] == 0 && j1 < largeuint.ndigits)
		j1++;

	int j2 = 0;
	if (largeuint.ndigits > MulExactNum)
	{
		j2 = largeuint.ndigits - MulExactNum;
	}
	int j = j1 > j2 ? j1 : j2;

	uint64_t* a = largeuint.digits + j;
	LargeUInt64 b(a, largeuint.ndigits - j);

	mantissa = b;
	exp = j;
}

BigFloat::BigFloat(uint64_t data[], int length, int mulexactnum) : sign(1), MulExactNum(mulexactnum), exp(0)
{
	mantissa = LargeUInt64(data, length);
}


BigFloat::BigFloat(BigFloat &bigfloat)
{
	MulExactNum = bigfloat.MulExactNum;
	exp = bigfloat.exp;
	sign = bigfloat.sign;
	mantissa = bigfloat.mantissa;
}
BigFloat::BigFloat(double d, int mulexactnum) : sign(1), exp(-1), MulExactNum(mulexactnum)
{
	mantissa = LargeUInt64(2);
	double f, i;
	f = modf(d, &i);
	mantissa.digits[1] = (uint64_t)(0.5 + i);
	mantissa.digits[0] = (uint64_t)(0.5 + f * pow(2, 64));
}

BigFloat::~BigFloat(void)
{
}

void BigFloat::Display(int n)
{
	int bakn = mantissa.ndigits;
	if (mantissa.ndigits > -exp)
		mantissa.ndigits = -exp;
	uint64_t base[] = { 10000000000000000000 };

	BigFloat bfbase(base, 1, MulExactNum);
	int len_dec = n / 19 + 1;
	uint64_t ans;
	BigFloat c = *this;
	mantissa.ndigits = bakn;
	int i;
	int count = 0;

	char output[256];

	int j = 0;
	for (i = 0; i <len_dec; i++)
	{
		c = c * bfbase;

		if ((c.mantissa.ndigits + c.exp) <= 0)
			ans = 0;
		else
		{
			ans = c.mantissa.digits[-c.exp];
			c.mantissa.ndigits--;
		}
		count += 19;
		if (j == 10)
		{
			for (int k = 0; k < 190; k++)
			{
				int num = output[k];
				if (num)
					putchar(num);
				else
					break;
				if ((k+1) % 10 == 0)
					printf("\n");
			}
			j = 0;
		}
		if (count < n)
			sprintf_s(output + j * 19, 21, "%019llu", ans);
		else
		{
			int last = n - count + 19;
			char* last_digits = new char[32];
			sprintf_s(last_digits, 21, "%019llu", ans);
			last_digits[last] = 0;
			sprintf_s(output + j * 19, 21, "%s", last_digits);

			delete[] last_digits;
			break;
		}
		j++;

		if (c.mantissa.ndigits == 0)
			break;
	}
	for (int k = 0; k < 190; k++)
	{
		int num = output[k];
		if (num)
			putchar(num);
		else
			break;
		if ((k + 1) % 10 == 0)
			printf("\n");
	}
}


string BigFloat::ftostr(int n)
{
	int bakn = mantissa.ndigits;
	if (mantissa.ndigits > -exp)
		mantissa.ndigits = -exp;
	uint64_t base[] = { 10000000000000000000 };
	LargeUInt64 lubase(base, 1);
	BigFloat bfbase(lubase, MulExactNum);
	int len_dec = n / 19 + 1;
	uint64_t* ans = new uint64_t[len_dec];
	BigFloat a = *this;
	mantissa.ndigits = bakn;
	int i;
	for (i = 0; i <len_dec; i++)
	{
		BigFloat c = a * bfbase;
		if ((c.mantissa.ndigits + c.exp) <= 0)
			ans[i] = 0;
		else
		{
			ans[i] = c.mantissa.digits[-c.exp];
			c.mantissa.ndigits--;
		}
			
		a = c;
		if (c.mantissa.ndigits == 0)
			break;
	}
	len_dec = i + 1;
	int len = len_dec * 19;

	char* digit = new char[len + 1];

	for (i = 0; i < len_dec; i++)
	{
		uint64_t tmp = ans[i];
		sprintf_s(digit + (i * 19), 21, "%019llu", tmp);
	}

	digit[i * 19] = 0;
	digit[n] = 0;
	string s(digit);
	return s;
}

BigFloat& BigFloat::operator= (const BigFloat &i)
{
	sign = i.sign;
	exp = i.exp;
	MulExactNum = i.MulExactNum;
	mantissa = i.mantissa;
	return *this;
}

BigFloat operator+ (const BigFloat &i1, const BigFloat &i2)
{
	int left1 = i1.exp;
	int left2 = i2.exp;
	int right1 = left1 + i1.mantissa.ndigits;
	int right2 = left2 + i2.mantissa.ndigits;
	int left = left1 < left2 ? left1 : left2;
	int right = right1 > right2 ? right1: right2;

	LargeUInt64 lui1(right - left);
	for (int i = 0; i < i1.mantissa.ndigits; i++)
	{
		lui1.digits[left1 - left + i] = i1.mantissa.digits[i];
	}
	LargeUInt64 lui2(right - left);
	for (int i = 0; i < i2.mantissa.ndigits; i++)
	{
		lui2.digits[left2 - left + i] = i2.mantissa.digits[i];
	}
	BigFloat ans(lui1 + lui2, i1.MulExactNum);
	ans.exp += left;

	return ans;
}

BigFloat operator- (const BigFloat &i1, const BigFloat &i2)
{
	int left1 = i1.exp;
	int left2 = i2.exp;
	int right1 = left1 + i1.mantissa.ndigits;
	int right2 = left2 + i2.mantissa.ndigits;
	int left = left1 < left2 ? left1 : left2;
	int right = right1 > right2 ? right1 : right2;

	LargeUInt64 lui1(right - left);
	for (int i = 0; i < i1.mantissa.ndigits; i++)
	{
		lui1.digits[left1 - left + i] = i1.mantissa.digits[i];
	}
	LargeUInt64 lui2(right - left);
	for (int i = 0; i < i2.mantissa.ndigits; i++)
	{
		lui2.digits[left2 - left + i] = i2.mantissa.digits[i];
	}

	LargeUInt64 tmp = lui1 - lui2;

	BigFloat ans(tmp, i1.MulExactNum);
	ans.exp += left;
	ans.sign = tmp.sign;

	return ans;
}


BigFloat operator* ( BigFloat &i1,  BigFloat &i2)
{
	LargeUInt64 multi = i1.mantissa * i2.mantissa;
	//cout << i1.mantissa.ndigits << "\t" << i2.mantissa.ndigits << "\t" << multi.ndigits << endl;

	int j1 = 0;
	while (multi.digits[j1] == 0 && j1 < multi.ndigits)
		j1++;

	int j2 = 0;
	if (multi.ndigits > i1.MulExactNum)
	{
		j2 = multi.ndigits - i1.MulExactNum;
	}
	int j = j1 > j2 ? j1 : j2;

	uint64_t* a = multi.digits + j;

	BigFloat ans(a, multi.ndigits - j, i1.MulExactNum);

	ans.sign = i1.sign * i2.sign;
	ans.exp = i1.exp + i2.exp + j;

	return ans;
}


BigFloat InverseSquareRoot(double C, int bits)
{
	int mulexactnum = (int)(bits / 19) + 2;

	int iter = (int)log2(bits);

	BigFloat x(C, mulexactnum);
	BigFloat init(1.0 / sqrt(C), mulexactnum);
	
	BigFloat rn = init;
	BigFloat tmp(0, mulexactnum);
	BigFloat tmp2(0, mulexactnum);
	BigFloat one((double)1, mulexactnum);
	BigFloat half((double)0.5, mulexactnum);
	//string s;
	for (int i = 0; i < iter; i++)
	{

		tmp = rn * rn * x;
		tmp = tmp - one;
		tmp2 = tmp* rn * half;

		if (tmp2.sign > 0)
		{
			tmp2.sign = 1;
			rn = rn - tmp2;
		}
			
		else
		{
			tmp2.sign = 1;
			rn = rn + tmp2;
		}


	}
	
	return rn;
}


BigFloat Reciprocal(BigFloat denom, int bits)
{
	int mulexactnum = (int)(bits / 19) + 2;
	int iter = (int)log2(bits);


	BigFloat x = denom;
	BigFloat init(1.0 / denom.mantissa.digits[denom.mantissa.ndigits - 1], mulexactnum);
	init.exp -= denom.mantissa.ndigits + denom.exp - 1;

	BigFloat rn = init;
	BigFloat tmp(0, mulexactnum);
	BigFloat tmp2(0, mulexactnum);
	BigFloat one((double)1, mulexactnum);
	//string s;
	for (int i = 0; i < iter; i++)
	{
		//s = rn.ftostr(bits);
		tmp = rn * x;
		tmp = tmp - one;
		//s = tmp.ftostr(bits);
		tmp2 = tmp* rn;

		if (tmp2.sign > 0)
		{
			tmp2.sign = 1;
			rn = rn - tmp2;
		}

		else
		{
			tmp2.sign = 1;
			rn = rn + tmp2;
		}
		//s = rn.ftostr(bits);

	}

	return rn;
}
