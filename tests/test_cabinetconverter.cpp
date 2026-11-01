/* This file contains simple unit tests for the cabinetconverter library
*
* Author: Josh McIntyre
*/
#include <cstdint>
#include <cstring>

#include "wavfile.h"
#include "irops.h"
#include "test_cabinetconverter.h"

// Test iteration calculations
bool test_resample()
{
	uint32_t convert_to_hz = 48000;
	uint32_t original_hz = 96000;
	uint32_t original_samples_length = 20;
	uint32_t expected_samples_length = 10;


	uint32_t fmt_chunk_size = 16;
	uint16_t audio_format = 1;
	uint16_t num_channels = 1;
	uint32_t samples_per_sec = uint32_t(original_hz);
	uint16_t block_align = 3;
	uint16_t bits_per_sample = 24;

	uint32_t expected_data_length = expected_samples_length * block_align;
	uint32_t expected_samples_per_sec = convert_to_hz;
	uint32_t expected_bytes_per_sec = convert_to_hz * block_align;
	uint32_t expected_chunk_size = uint32_t(RIFF_MAGIC_NUMBER + (expected_samples_length * block_align));

	WavHeader wav_header{};
	std::memcpy(wav_header.riffMagicHeader, "RIFF", 4);
	wav_header.chunkSize = uint32_t(RIFF_MAGIC_NUMBER + (original_samples_length * block_align));
	std::memcpy(wav_header.waveHeader, "WAVE", 4);
	std::memcpy(wav_header.fmtHeader, "fmt ", 4);
	wav_header.fmtChunkSize = fmt_chunk_size;
	wav_header.audioFormat = audio_format;
	wav_header.numChannels = num_channels;
	wav_header.samplesPerSec = original_hz;
    wav_header.bytesPerSec = uint32_t(original_hz * block_align);
	wav_header.blockAlign = block_align;
	wav_header.bitsPerSample = bits_per_sample;
	std::memcpy(wav_header.dataString, "data", 4);
    wav_header.dataLength = uint32_t(original_samples_length * block_align);

	std::vector<float> original_samples = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200 };
	std::vector<float> expected_re_samples = { 10, 30, 50, 70, 90, 110, 130, 150, 170, 190 };
	std::vector<float> re_samples = resample(convert_to_hz, wav_header, original_samples);

	bool result = true;

	if (re_samples != expected_re_samples)
	{
		std::cout << "re_samples values do not match expected" << std::endl;

		std::cout << "re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << re_samples[i] << ", ";
		}
		std::cout << std::endl;

		std::cout << "expected_re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << expected_re_samples[i] << ", ";
		}
		std::cout << std::endl;

		result = false;
	}

	if (wav_header.samplesPerSec != expected_samples_per_sec)
	{
		std::cout << "wav_header.samplesPerSec got: " << wav_header.samplesPerSec << " expected: " << expected_samples_per_sec << std::endl;

		result = false;
	}

	if (wav_header.bytesPerSec != expected_bytes_per_sec)
	{
		std::cout << "wav_header.bytesPerSec got: " << wav_header.bytesPerSec << " expected: " << expected_bytes_per_sec << std::endl;

		result = false;
	}

	if (wav_header.dataLength != expected_data_length)
	{
		std::cout << "wav_header.dataLength got: " << wav_header.dataLength << " expected: " << expected_data_length << std::endl;

		result = false;
	}

	if (wav_header.chunkSize != expected_chunk_size)
	{
		std::cout << "wav_header.chunkSize got: " << wav_header.chunkSize << " expected: " << expected_chunk_size << std::endl;

		result = false;
	}

	return result;
}

bool test_resample_flat()
{
	uint32_t convert_to_hz = 48000;
	uint32_t original_hz = 96000;
	uint32_t original_samples_length = 20;
	uint32_t expected_samples_length = 10;


	uint32_t fmt_chunk_size = 16;
	uint16_t audio_format = 1;
	uint16_t num_channels = 1;
	uint32_t samples_per_sec = uint32_t(original_hz);
	uint16_t block_align = 3;
	uint16_t bits_per_sample = 24;

	uint32_t expected_data_length = expected_samples_length * block_align;
	uint32_t expected_samples_per_sec = convert_to_hz;
	uint32_t expected_bytes_per_sec = convert_to_hz * block_align;
	uint32_t expected_chunk_size = uint32_t(RIFF_MAGIC_NUMBER + (expected_samples_length * block_align));

	WavHeader wav_header{};
	std::memcpy(wav_header.riffMagicHeader, "RIFF", 4);
	wav_header.chunkSize = uint32_t(RIFF_MAGIC_NUMBER + (original_samples_length * block_align));
	std::memcpy(wav_header.waveHeader, "WAVE", 4);
	std::memcpy(wav_header.fmtHeader, "fmt ", 4);
	wav_header.fmtChunkSize = fmt_chunk_size;
	wav_header.audioFormat = audio_format;
	wav_header.numChannels = num_channels;
	wav_header.samplesPerSec = original_hz;
    wav_header.bytesPerSec = uint32_t(original_hz * block_align);
	wav_header.blockAlign = block_align;
	wav_header.bitsPerSample = bits_per_sample;
	std::memcpy(wav_header.dataString, "data", 4);
    wav_header.dataLength = uint32_t(original_samples_length * block_align);

	std::vector<float> original_samples = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
	std::vector<float> expected_re_samples = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, };
	std::vector<float> re_samples = resample(convert_to_hz, wav_header, original_samples);

	bool result = true;

	if (re_samples != expected_re_samples)
	{
		std::cout << "re_samples values do not match expected" << std::endl;

		std::cout << "re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << re_samples[i] << ", ";
		}
		std::cout << std::endl;

		std::cout << "expected_re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << expected_re_samples[i] << ", ";
		}
		std::cout << std::endl;

		result = false;
	}

	if (wav_header.samplesPerSec != expected_samples_per_sec)
	{
		std::cout << "wav_header.samplesPerSec got: " << wav_header.samplesPerSec << " expected: " << expected_samples_per_sec << std::endl;

		result = false;
	}

	if (wav_header.bytesPerSec != expected_bytes_per_sec)
	{
		std::cout << "wav_header.bytesPerSec got: " << wav_header.bytesPerSec << " expected: " << expected_bytes_per_sec << std::endl;

		result = false;
	}

	if (wav_header.dataLength != expected_data_length)
	{
		std::cout << "wav_header.dataLength got: " << wav_header.dataLength << " expected: " << expected_data_length << std::endl;

		result = false;
	}

	if (wav_header.chunkSize != expected_chunk_size)
	{
		std::cout << "wav_header.chunkSize got: " << wav_header.chunkSize << " expected: " << expected_chunk_size << std::endl;

		result = false;
	}

	return result;
}

bool test_resample_interpolate()
{
	uint32_t convert_to_hz = 44100;
	uint32_t original_hz = 48000;
	uint32_t original_samples_length = 15;
	uint32_t expected_samples_length = 14;

	uint32_t fmt_chunk_size = 16;
	uint16_t audio_format = 1;
	uint16_t num_channels = 1;
	uint32_t samples_per_sec = uint32_t(original_hz);
	uint16_t block_align = 3;
	uint16_t bits_per_sample = 24;

	uint32_t expected_data_length = expected_samples_length * block_align;
	uint32_t expected_samples_per_sec = convert_to_hz;
	uint32_t expected_bytes_per_sec = convert_to_hz * block_align;
	uint32_t expected_chunk_size = uint32_t(RIFF_MAGIC_NUMBER + (expected_samples_length * block_align));

	WavHeader wav_header{};
	std::memcpy(wav_header.riffMagicHeader, "RIFF", 4);
	wav_header.chunkSize = uint32_t(RIFF_MAGIC_NUMBER + (original_samples_length * block_align));
	std::memcpy(wav_header.waveHeader, "WAVE", 4);
	std::memcpy(wav_header.fmtHeader, "fmt ", 4);
	wav_header.fmtChunkSize = fmt_chunk_size;
	wav_header.audioFormat = audio_format;
	wav_header.numChannels = num_channels;
	wav_header.samplesPerSec = original_hz;
    wav_header.bytesPerSec = uint32_t(original_hz * block_align);
	wav_header.blockAlign = block_align;
	wav_header.bitsPerSample = bits_per_sample;
	std::memcpy(wav_header.dataString, "data", 4);
    wav_header.dataLength = uint32_t(original_samples_length * block_align);

	std::vector<float> original_samples = { 10, 20, 30, 40, 50, 10, 20, 30, 40, 50, 10, 20, 30, 40, 50 };
	std::vector<float> expected_re_samples = { 10, 20.8844, 31.7687, 42.6531, 35.8503, 14.4218, 25.3061, 36.1905, 47.0748, 18.1633, 18.8435, 29.7279, 40.6123, 50 };
	std::vector<float> re_samples = resample(convert_to_hz, wav_header, original_samples);

	bool result = true;

	// With floating point results, we have to calculate equality with a small expected delta between values
	bool vector_equal = true;
	for (int i = 0; i < expected_samples_length; i++)
	{
		float delta = std::abs(expected_re_samples[i] - re_samples[i]);
		if (delta > FLOAT_EPSILON)
		{
			std::cout << std::setprecision(6) <<  "Delta: " << delta << " between " << 
				expected_re_samples[i] << " and " << re_samples[i] << " greater than allowable " 
				<< FLOAT_EPSILON << std::endl;
			vector_equal = false;
		}
	}

	if ( !vector_equal )
	{
		std::cout << "re_samples values do not match expected" << std::endl;

		std::cout << "re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << std::setprecision(6) << re_samples[i] << ", ";
		}
		std::cout << std::endl;

		std::cout << "expected_re_samples: ";
		for (int i = 0; i < expected_samples_length; i++)
		{
			std::cout << std::setprecision(6) << expected_re_samples[i] << ", ";
		}
		std::cout << std::endl;

		result = false;
	}

	if (wav_header.samplesPerSec != expected_samples_per_sec)
	{
		std::cout << "wav_header.samplesPerSec got: " << wav_header.samplesPerSec << " expected: " << expected_samples_per_sec << std::endl;

		result = false;
	}

	if (wav_header.bytesPerSec != expected_bytes_per_sec)
	{
		std::cout << "wav_header.bytesPerSec got: " << wav_header.bytesPerSec << " expected: " << expected_bytes_per_sec << std::endl;

		result = false;
	}

	if (wav_header.dataLength != expected_data_length)
	{
		std::cout << "wav_header.dataLength got: " << wav_header.dataLength << " expected: " << expected_data_length << std::endl;

		result = false;
	}

	if (wav_header.chunkSize != expected_chunk_size)
	{
		std::cout << "wav_header.chunkSize got: " << wav_header.chunkSize << " expected: " << expected_chunk_size << std::endl;

		result = false;
	}

	return result;
}

// The main entry point for the unit tests
int main(int argc, char* argv[])
{
	int tests = 3;
	bool results[3];

	bool res_resample = test_resample();
	results[0] = res_resample;

	bool res_resample_flat = test_resample_flat();
	results[1] = res_resample_flat;

	bool res_resample_interpolate = test_resample_interpolate();
	results[2] = res_resample_interpolate;
	
	int tests_passed = 0;
	int tests_failed = 0;
	for (int i = 0; i < tests; i++)
	{
		if (results[i] == true)
		{
			tests_passed++;
		}
		else
		{
			tests_failed++;
		}
	}
	
	std::cout << "-----" << std::endl;
	std::cout << "Tests passed: " << tests_passed << std::endl;
	std::cout << "Tests failed: " << tests_failed << std::endl;
	
	return 0;
}