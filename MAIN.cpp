#include <iostream>

#include "fileManager.h"
#include "Demodulator.h"

int main() {

    std::pair <std::string, DataType> AM = std::make_pair("D:\\DataSource\\am_sound.dat", DataType::INT);

    fileManager AM_Data(AM);
    Demodulator AMSignal(AM_Data.getRawData(), Demodulator_Mode::AM, -2.25, 5);

    AM_Data.addSignal(AMSignal.getSignal());
    AM_Data.SaveMusic("D:\\OutMusic\\TestAM.wav");


    std::pair <std::string, DataType> FM = std::make_pair("D:\\DataSource\\file1EuropaPlus.bin", DataType::FLOAT32);

    fileManager FM_Data(FM);
    Demodulator FMSignal(FM_Data.getRawData(), Demodulator_Mode::FM, 15.625, 15);

    FM_Data.addSignal(FMSignal.getSignal());
    FM_Data.SaveMusic("D:\\OutMusic\\TestFM.wav");

}