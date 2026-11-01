/* This file contains function definitions and includes for the main program
*
* Author: Josh McIntyre
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "irops.h"

// Useful constants
const int CONVERT_TO_44100_HZ = 44100;
const int CONVERT_TO_48000_HZ = 48000;
const int CONVERT_TO_88200_HZ = 88200;
const int CONVERT_TO_96000_HZ = 96000;

const int CONVERSION_OPTIONS[4] = { CONVERT_TO_44100_HZ, CONVERT_TO_48000_HZ, CONVERT_TO_88200_HZ, CONVERT_TO_96000_HZ };
const std::string CONVERSION_OPTIONS_COMMA_STR = "44100, 48000, 88200, 96000";

void output_help();
