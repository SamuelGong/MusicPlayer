#pragma once
#include <complex>
using namespace std;

class FastFourierTransform {
	friend class Spectrum;
private:
	int count; // �������������
public:
	FastFourierTransform(int c) { count = c; }
	// ���ٸ���Ҷ�任���Ĳ���
	void process(complex<float>*Data, int Log2N, int sign);
};


