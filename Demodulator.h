#pragma once

#include <fstream>
#include <iostream>

#include <vector>
#include <complex>

enum class Demodulator_Mode { AM = 1, FM = 2 };

class Demodulator
{
public:

	Demodulator(const std::vector<std::complex<float>>& Source, Demodulator_Mode mode, const float ReSample_step, const size_t FilterWindow_size);
	~Demodulator();

	void Delete();

	void DemodulateAM(const std::vector<std::complex<float>>& Source, const float ReSample_step, const size_t FilterWindow_size);
	void DemodulateFM(const std::vector<std::complex<float>>& Source, const float ReSample_step, const size_t FilterWindow_size);

	void ReSample(const float step);
	void Filter(const size_t window);

	std::vector<float> getSignal();

private:
	std::vector<float> Signal;
};