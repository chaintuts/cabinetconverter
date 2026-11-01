/* This file contains code for re-sampling a cabinet/speaker impulse response (IR) to a different sample rate
* For example, re-sampling the IR from 48 KHz to 44.1 KHz
* This file contains the main entry point for the program
* 
* Author: Josh McIntyre
*/

#include "cabinetconverter.h"

// This function is the main entry point for the program
int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		output_help();
		exit(1);
	}

	std::string input_filename = std::string(argv[1]);
	std::string output_filename = std::string(argv[2]);
	std::string convert_to_hz_str = std::string(argv[3]);

	int convert_to_hz = std::stoi(convert_to_hz_str);

	if (std::find(std::begin(CONVERSION_OPTIONS), std::end(CONVERSION_OPTIONS), convert_to_hz) == std::end(CONVERSION_OPTIONS))
	{
		std::cout << "Unsupported sample rate: " << convert_to_hz << std::endl;
		std::cout << "Supported sample rates are: " << CONVERSION_OPTIONS_COMMA_STR << std::endl;
		exit(1);
	}

	wav_convert(input_filename, output_filename, convert_to_hz);

	return 0;
}

void output_help()
{
	std::cout << "Usage: ./cabinetconverter <input_filename> <output_filename> <convert_to_hz>";
}

