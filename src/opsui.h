/* This file contains includes and function definitions for simple output code
*
* Author: Josh McIntyre
*/

#include <iostream>
#include <vector>
#include <string_view>

#include "wavfile.h"

// Output constants
const std::string FILE_OP_INPUT = "Input";
const std::string FILE_OP_OUTPUT = "Output";
const std::string UNSUPPORTED_BIT_DEPTH_EXCEPTION = "Unsupported bit depth";

// Function definitions
void output_wavfile_info(std::string filename, std::string file_op, WavHeader &wav_header, std::vector<float> &wav_samples);
