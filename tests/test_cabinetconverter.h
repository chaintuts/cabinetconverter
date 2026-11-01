/* This file contains includes and prototypes for the main command line UI
*
* Author: Josh McIntyre
*/
#include <iostream>
#include <iomanip>
#include <string>

// Constants
const float FLOAT_EPSILON = 0.0001f;

// Function prototypes
bool test_resample();
bool test_resample_flat();
bool test_resample_interpolate();