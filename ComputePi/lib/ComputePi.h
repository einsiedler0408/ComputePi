#ifndef ComputePi_H_
#define ComputePi_H_

#include "Global.h"
#include "LargeUInt64.h"
#include "BigFloat.h"

struct PQT{
	LargeUInt64 P;
	LargeUInt64 Q;
	LargeUInt64 T;
};


class ComputePi
{
public:
	int precision;
	const uint64_t A = 13591409;
	const uint64_t B = 545140134;
	const double C1 = 640320;
	const uint64_t C2 = 10939058860032000;
	const double C3 = 34167475200;


	ComputePi(int n);
	~ComputePi(void);

	void compute(void);

private:
	PQT ComputePQT(int n1, int n2);



};


#endif //ComputePi_H_