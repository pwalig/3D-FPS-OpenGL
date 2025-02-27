#pragma once
#include <fstream>
#include <vector>
#define allocation_limit 10485760 // 10 MB

namespace mesh_reader {

	template <typename T, size_t S>
	struct big_endian_reader {
		inline T operator()(std::istream& input) {
			static_assert(0);
		}
	};

	template <typename T>
	struct big_endian_reader<T, 1> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 1);
			T val;
			input.read((char*)(&val), 1);
			return val;
		}
	};

	template <typename T>
	struct big_endian_reader<T, 2> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 2);
			unsigned char data[2];
			input.read((char*)data, 2);
			uint16_t mem = data[1] | ((unsigned)data[0] << 8);
			return *((T*)(&mem));
		}
	};

	template <typename T>
	struct big_endian_reader<T, 4> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 4);
			unsigned char data[4];
			input.read((char*)data, 4);
			uint32_t mem = data[3] | (data[2] << 8) | (data[1] << 16) | ((unsigned)data[0] << 24);
			return *((T*)(&mem));
		}
	};

	template <typename T>
	struct big_endian_reader<T, 8> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 8);
			unsigned char data[8];
			input.read((char*)data, 8);
			uint64_t mem = data[7] | (data[6] << 8) | (data[5] << 16) | (data[4] << 24)
				| (data[3] << 32) | (data[2] << 40) | (data[1] << 48) | ((unsigned)data[0] << 56);
			return *((T*)(&mem));
		}
	};

	template<typename T>
	inline T read_big_endian(std::istream& input) {
		return big_endian_reader<T, sizeof(T)>()(input);
	}

	template <typename T, size_t S>
	struct little_endian_reader {
		inline T operator()(std::istream& input) {
			static_assert(0);
		}
	};

	template <typename T>
	struct little_endian_reader<T, 1> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 1);
			T val;
			input.read((char*)(&val), 1);
			return val;
		}
	};

	template <typename T>
	struct little_endian_reader<T, 2> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 2);
			unsigned char data[2];
			input.read((char*)data, 2);
			uint16_t mem = data[0] | ((unsigned)data[1] << 8);
			return *((T*)(&mem));
		}
	};

	template <typename T>
	struct little_endian_reader<T, 4> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 4);
			unsigned char data[4];
			input.read((char*)data, 4);
			uint32_t mem = ((unsigned)data[0]) | ((unsigned)data[1] << 8) | ((unsigned)data[2] << 16) | ((unsigned)data[3] << 24);
			return *((T*)(&mem));
		}
	};

	template <typename T>
	struct little_endian_reader<T, 8> {
		inline T operator()(std::istream& input) {
			static_assert(sizeof(T) == 8);
			unsigned char data[8];
			input.read((char*)data, 8);
			uint64_t mem = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24)
				| (data[4] << 32) | (data[5] << 40) | (data[6] << 48) | ((unsigned)data[7] << 56);
			return *((T*)(&mem));
		}
	};

	template<typename T>
	inline T read_little_endian(std::istream& input) {
		return little_endian_reader<T, sizeof(T)>()(input);
	}


	template <typename T, typename U>
	inline void read_big_endian_buffer(std::ifstream& file, std::vector<T>& buffer, const U& count) {
#ifdef allocation_limit
		if (count * sizeof(T) > allocation_limit) throw std::runtime_error("allocation limit exceeded");
#endif
		buffer.reserve(count);
		for (U i = 0; i < count; ++i) {
			buffer.push_back(read_big_endian<T>(file));
		}
	}

	// assumes count of type T values at front as type U value
	template <typename T, typename U>
	inline void read_big_endian_buffer(std::ifstream& file, std::vector<T>& buffer) {
		U count = read_big_endian<U>(file);
		read_big_endian_buffer<T, U>(file, buffer, count);
	}


	template <typename T, typename U>
	inline void read_little_endian_buffer(std::ifstream& file, std::vector<T>& buffer, const U& count) {
#ifdef allocation_limit
		if (count * sizeof(T) > allocation_limit) throw std::runtime_error("allocation limit exceeded");
#endif
		buffer.reserve(count);
		for (U i = 0; i < count; ++i) {
			buffer.push_back(read_little_endian<T>(file));
		}
	}

	// assumes count of type T values at front as type U value
	template <typename T, typename U>
	inline void read_little_endian_buffer(std::ifstream& file, std::vector<T>& buffer) {
		U count = read_little_endian<U>(file);
		read_little_endian_buffer<T, U>(file, buffer, count);
	}

	template <typename T, typename U>
	inline void read_native_endian_buffer(std::ifstream& file, std::vector<T>& buffer, const U& count) {
#ifdef allocation_limit
		if (count * sizeof(T) > allocation_limit) throw std::runtime_error("allocation limit exceeded");
#endif
		buffer.resize(count);
		file.read((char*)(buffer.data()), count * sizeof(T));
	}

	// assumes count of type T values at front as type U value
	template <typename T, typename U>
	inline void read_native_endian_buffer(std::ifstream& file, std::vector<T>& buffer) {
		U count;
		file.read((char*)(&count), sizeof(U));
		read_native_endian_buffer<T, U>(file, buffer, count);
	}
}

