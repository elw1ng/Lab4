#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct riff_header {
	int32_t chunk_id;
	int32_t chunk_size;
	int32_t format;
};

struct sub_chunk_1 {
	int32_t sub_chunk_1_id;
	int32_t sub_chunk_1_size;
	int16_t audio_format;
	int16_t num_channels;
	int32_t sample_rate;
	int32_t byte_rate;
	int16_t block_align;
	int16_t bits_per_sample;
};

struct sub_chunk_2 {
	int32_t sub_chunk_2_id;
	int32_t sub_chunk_2_size;
};

class music
{
public:

	music() : num_samples_(0), data_(nullptr) {}

	explicit music(const string& filename) : num_samples_(0), data_(nullptr) {
		read(filename);
	}
	void read(const string& filename)
	{
		ifstream in(filename, ios::binary);
		if (in.is_open())
		{
			in.read(reinterpret_cast<char*>(&riff_), sizeof riff_);
			in.read(reinterpret_cast<char*>(&chunk_1_), sizeof chunk_1_);
			in.read(reinterpret_cast<char*>(&chunk_2_), sizeof chunk_2_);

			num_samples_ = 8 * chunk_2_.sub_chunk_2_size /
				chunk_1_.num_channels / chunk_1_.bits_per_sample;

			data_ = new int16_t[num_samples_];

			for (auto i = 0; i < num_samples_; ++i) {
				in.read(reinterpret_cast<char*>(&data_[i]), chunk_1_.bits_per_sample / 8);
			}

			in.close();
		}
		else cerr << "Can not open the file " + filename;
	}

	void write(const string& filename)
	{
		ofstream out(filename, ios::binary);
		
		if (out.is_open()) 
		{
			out.write(reinterpret_cast<char*>(&riff_), sizeof riff_);
			out.write(reinterpret_cast<char*>(&chunk_1_), sizeof chunk_1_);
			out.write(reinterpret_cast<char*>(&chunk_2_), sizeof chunk_2_);

			for (auto i = 0; i < num_samples_; ++i) {
				out.write(reinterpret_cast<char*>(&data_[i]), chunk_1_.bits_per_sample / 8);
			}

			out.close();
		}
		else cerr << "Can not open the file " + filename;
	}

private:

	struct indices {
		indices() = default;

		explicit indices(const double* values) {
			a = values[0]; b = values[1];
			c = values[2]; d = values[3];
		}

		double a, b, c, d;
	};
	
	double* gauss(double** matrix, double* results, const int size) const {
		auto* values = new double[size];
		for (auto i = 0; i < size; ++i) {
			auto value = matrix[i][i];
			if (value == 0) {
				auto local = i + 1;
				while (value == 0) {
					value = matrix[local++][i];
				}
				swap(matrix[i], matrix[--local]);
				swap(results[i], results[local]);
			}
			for (auto j = i + 1; j < size; ++j) {
				const auto index = -matrix[j][i] / value;
				for (auto k = i; k < size; ++k) {
					matrix[j][k] += matrix[i][k] * index;
				}
				results[j] += results[i] * index;
			}
		}
		for (auto i = size - 1; i >= 0; --i) {
			const auto index = matrix[i][i];
			auto result = results[i];
			for (auto j = i + 1; j < size; ++j) {
				result -= matrix[i][j] * values[j];
			}
			values[i] = result / index;
		}
		return values;
	}

	template<typename T>
	static void delete_matrix(T** matrix, const int size) {
		for (auto i = 0; i < size; ++i) {
			delete[] matrix[i];
		}
		delete[] matrix;
	}
	
	const int index_count_ = 4;
	riff_header riff_ = {};
	sub_chunk_1 chunk_1_ = {};
	sub_chunk_2 chunk_2_ = {};
	int num_samples_;
	int16_t* data_;
};
int main()
{
	return 0;
}
