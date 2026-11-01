/* This file contains function definitions and includes for reading and parsing a wav file, resampling, and writing
*
* Author: Josh McIntyre
*/
#include <cmath>
#include <cstdint>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>

#include "opsui.h"
#include "wavfile.h"

// Constants
const int RIFF_MAGIC_NUMBER = 36;

// Function definitions
void read_wav(std::string input_filename, WavHeader &wav_header, std::vector<float> &wav_samples);
void write_wav(std::string output_filename, WavHeader &wav_header, std::vector<float> &wav_samples);

std::vector<float> resample(int convert_to_hz, WavHeader &wav_header, std::vector<float> &original_samples);

void wav_convert(std::string input_filename, std::string output_filename, int convert_to_hz);