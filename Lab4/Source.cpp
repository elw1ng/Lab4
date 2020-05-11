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

private:
	
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
