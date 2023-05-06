#include "fileManager.h"

fileManager::fileManager(const std::pair < std::string, DataType>& key) {

	RawData.clear();
	RawData.shrink_to_fit();

	std::ifstream source(key.first, std::ios::binary);

	if (source.is_open()) {

		std::cout << "Opening file: " << key.first << ", was SUCCESFULL " << '\n';
		switch (key.second)
		{
		case DataType::INT16:
			std::cout << "Reading data type: int16" << '\n';
			Read_INT16(source, key);		
			break;

		case DataType::INT:
			std::cout << "Reading data type: int" << '\n';
			Read_INT(source, key);
			break;

		case DataType::FLOAT32:
			std::cout << "Reading data type: float32" << '\n';
			Read_FLOAT32(source, key);
			break;
		}	
	}
	else
		std::cout << "ERROR of opening file: " << key.first << '\n';
	source.close();
}
fileManager::~fileManager() {

	RawData.clear();
	RawData.shrink_to_fit();

	ProcessedData.clear();
	ProcessedData.shrink_to_fit();
}

void fileManager::Delete() {
	fileManager::~fileManager();
}

void fileManager::addSignal(const std::vector<float>& Source) {

	ProcessedData.clear();
	ProcessedData.shrink_to_fit();

	float max = 0;
	for (size_t i = 0; i < Source.size(); ++i)
		if (abs(Source[i]) > max)   max = abs(Source[i]);

	for(size_t i = 0; i < Source.size(); ++i)
		ProcessedData.push_back(int16_t(Source[i]*32767/max));
}
void fileManager::SaveMusic(const std::string& key) {

	static_assert(sizeof(wav_hdr) == 44, "");
	uint32_t fsize = ProcessedData.size() * 4;
	size_t size = sizeof(int16_t);

	wav_hdr wav;
	wav.ChunkSize = fsize + sizeof(wav_hdr) - 8;
	wav.Subchunk2Size = fsize + sizeof(wav_hdr) - 44;

	std::ofstream Music(key, std::ios::binary);
	Music.write(reinterpret_cast<const char*>(&wav), sizeof(wav));
	int16_t data;

	for (size_t i = 0; i < ProcessedData.size(); ++i) {
		data = ProcessedData[i];
		Music.write(reinterpret_cast<char*>(&data), size);
	}

	std::cout << "Music was saved!" << '\n';
}

void fileManager::Read_INT16(std::ifstream& source, const std::pair < std::string, DataType>& key) {

	int16_t Real, Imag;
	size_t size = sizeof(int16_t);

	while (!source.eof()) {
		source.read((char*)&Real, size);
		source.read((char*)&Imag, size);
		std::complex<float> Num(Real, Imag);
		RawData.push_back(Num);
	}
}
void fileManager::Read_INT(std::ifstream& source, const std::pair < std::string, DataType>& key) {

	int Real, Imag;
	size_t size = sizeof(int);

	while (!source.eof()) {
		source.read((char*)&Real, size);
		source.read((char*)&Imag, size);	
		RawData.push_back(std::complex<float>(Real, Imag));
	}	
}
void fileManager::Read_FLOAT32(std::ifstream& source, const std::pair < std::string, DataType>& key) {

	float Real, Imag;
	size_t size = sizeof(float);

	while (!source.eof()) {
		source.read((char*)&Real, size);
		source.read((char*)&Imag, size);
		std::complex<float> Num(Real, Imag);
		RawData.push_back(Num);
	}
}

std::vector<std::complex<float>> fileManager::getRawData() {
	return RawData;
}