#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <complex>

#include "WAVHeader.h"

enum class DataType { INT16 = 1, INT = 2, FLOAT32 = 3 };

class fileManager
{
public:

	fileManager(const std::pair < std::string, DataType>& key);
	~fileManager();

	void Delete();

	void addSignal(const std::vector<float>& Source);
	void SaveMusic(const std::string& key);

	void Read_INT16(std::ifstream& source, const std::pair < std::string, DataType>& key);
	void Read_INT(std::ifstream& source, const std::pair < std::string, DataType>& key);
	void Read_FLOAT32(std::ifstream& source, const std::pair < std::string, DataType>& key);

	std::vector<std::complex<float>> getRawData();

private:
	std::vector<std::complex<float>> RawData;
	std::vector<int16_t> ProcessedData;
};