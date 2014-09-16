#include "..\lib\ComputePi.h"




ComputePi::ComputePi(int n)
{
	precision = n;
}

ComputePi::~ComputePi(void)
{

}


PQT ComputePi::ComputePQT(int n1, int n2)
{
	if (n1 + 1 == n2)
	{
		PQT a;
		a.P = LargeUInt64((uint64_t)(2 * n2 - 1)) * LargeUInt64((uint64_t)6 * n2 - 5) *  LargeUInt64((uint64_t)6 * n2 - 1);
		a.Q = power(n2, 3) * LargeUInt64((uint64_t)C2);
		a.T = LargeUInt64((uint64_t)A + B * n2) * a.P;
		if (n2 & 0x1)
			a.T.sign = -1;
		return a;
	}
	int m = (n1 + n2) / 2;
	PQT a1 = ComputePQT(n1, m);
	PQT a2 = ComputePQT(m, n2);
	
	PQT a;
	a.P = a1.P * a2.P;
	a.Q = a1.Q * a2.Q;
	LargeUInt64 tmp2 = a1.P * a2.T;
	LargeUInt64 tmp1 = a1.T * a2.Q;
	if (tmp1.sign == tmp2.sign)
	{
		a.T = tmp1 + tmp2;
		a.T.sign = tmp1.sign;
	}
	else
	{
		a.T = tmp1 - tmp2;
		a.T.sign *= tmp1.sign;
	}
	
		
	return a;
}



void ComputePi::compute(void)
{
	int iter = (precision / 14);
	int mulexactnum = (int)(precision / 19) + 2;

	PQT a = ComputePQT(0, iter);

	BigFloat tmp1 = InverseSquareRoot(C1, precision);


	BigFloat Q0N(a.Q, mulexactnum);
	BigFloat tmp2 = BigFloat((double)A, mulexactnum) * Q0N;
	if (a.T.sign > 0)
		tmp2 = tmp2 + BigFloat(a.T, mulexactnum);
	else
		tmp2 = tmp2 - BigFloat(a.T, mulexactnum);
	
	BigFloat down = Reciprocal(tmp2, precision);

	BigFloat pi = Q0N * down * BigFloat(C3, mulexactnum) * tmp1;
	pi.Display(precision);

}

int main(int argc, char *argv[])
{
	//_CrtSetBreakAlloc(385013);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//clock_t start = clock();
	
	ComputePi test(atoi(argv[1]));
	test.compute();
	//clock_t end = clock();
	//cout << endl << (double)(end - start) / CLOCKS_PER_SEC << endl;
}