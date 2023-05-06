#include "Demodulator.h"

Demodulator::Demodulator(const std::vector<std::complex<float>>& Source, Demodulator_Mode mode, const float ReSample_step, const size_t FilterWindow_size) {
    
    Signal.clear();
    Signal.shrink_to_fit();

    switch (mode) 
    {
        case Demodulator_Mode::AM:
            std::cout << "Start AM demodulation : ";
            DemodulateAM(Source, ReSample_step, FilterWindow_size);
            std::cout << "DONE" << '\n';
        break;

        case Demodulator_Mode::FM:
            std::cout << "Start FM demodulation : ";
            DemodulateFM(Source, ReSample_step, FilterWindow_size);
            std::cout << "DONE" << '\n';
        break;
    }

}
Demodulator::~Demodulator() {
    Signal.clear();
    Signal.shrink_to_fit();
}

void Demodulator::Delete() {
    Demodulator::~Demodulator();
}

void Demodulator::DemodulateAM(const std::vector<std::complex<float>>& Source, const float ReSample_step,const size_t FilterWindow_size) {

    for (size_t i = 0; i < Source.size(); ++i)
        Signal.push_back(abs(Source[i]));

    ReSample(ReSample_step);
    Filter(FilterWindow_size);  
}
void Demodulator::DemodulateFM(const std::vector<std::complex<float>>& Source, const float ReSample_step, const size_t FilterWindow_size) {

    float A0 = Source[0].real();
    float B0 = Source[0].imag();

    for (size_t i = 1; i < Source.size(); ++i){

        float A1 = Source[i].real();
        float B1 = Source[i].imag();

        float dA = A1 - A0;
        float dB = B1 - B0;

        float w = (dB * A1 - dA * B1) / (A1 * A1 + B1 * B1);

        Signal.push_back(w);

        A0 = A1;
        B0 = B1;
    }

    Filter(FilterWindow_size);
    ReSample(ReSample_step);
}

void Demodulator::ReSample(const float step) {

    std::vector<float> Byffer;

    size_t intPart = abs(int(step));
    float remains = abs(fmod(step, 1));

    float raid = 0;
    bool Fract = false;
    if (abs(step) != intPart) Fract = true;

    if (step > 1) {

        for (size_t i = 0; i < Signal.size(); i += intPart) {
            Byffer.push_back(Signal[i]);

            if (Fract) {
                raid += remains;

                if (raid >= 0.99) {
                    ++i;
                    if (raid >= 1) raid -= 1;
                    else raid = 0;
                }
            }
        }
    }

    else if (step < -1) {

        for (size_t i = 0; i < Signal.size(); ++i)
            for (size_t j = 0; j < intPart; ++j) {
                Byffer.push_back(Signal[i]);

                if (Fract) {
                    raid += remains;

                    if (raid >= 0.99) {
                        --j;
                        if (raid >= 1) raid -= 1;
                        else raid = 0;
                    }
                }
            }
    }

    else Byffer = Signal;

    Signal = Byffer;
    Byffer.clear();
}
void Demodulator::Filter(const size_t window) {
    
    if (window > 1){

        std::vector<float> Byffer;
        size_t length = window - 1;
        float sum = 0;
 
        for (size_t i = 0; i < Signal.size(); ++i) {
            if (i < length){
                Byffer.push_back(0);
                sum += Signal[i];
            }
            else {
                sum += Signal[i];
                Byffer.push_back(sum/ window);
                sum -= Signal[i - length];
            }
        }

        Signal = Byffer;
        Byffer.clear();
    }
}


std::vector<float> Demodulator::getSignal() {
    return Signal;
}