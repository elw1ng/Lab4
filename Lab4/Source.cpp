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
class music {

public:

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

}
