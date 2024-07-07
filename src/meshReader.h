#pragma once
#include <fstream>
#include <vector>

#define mesh_reader_count_limit 200000

namespace mesh_reader {
	template <typename T, typename U>
	void readBuffer(std::ifstream& file, std::vector<T>& buffer); // assumes count of type T values at front as type U value

	template <typename T, typename U>
	void readBuffer(std::ifstream& file, std::vector<T>& buffer, const U& count);
}

template<typename T, typename U>
void mesh_reader::readBuffer(std::ifstream& file, std::vector<T>& buffer)
{
	U count;
	file.read((char*)&count, sizeof(U));
	if (count > mesh_reader_count_limit) {
		printf("mesh reader: allocation limit exceded could not read file count was: %d\n", count);
		return;
	}
	readBuffer<T, U>(file, buffer, count);
}

template<typename T, typename U>
void mesh_reader::readBuffer(std::ifstream& file, std::vector<T>& buffer, const U& count)
{
	if (count > mesh_reader_count_limit) {
		printf("mesh reader: allocation limit exceded could not read file count was: %d\n", count);
		return;
	}
	buffer.resize(count);
	file.read((char*)(buffer.data()), count * sizeof(T));
	if (!file.good()) printf("mesh reader: reading problems\n");
}