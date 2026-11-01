/* This file contains structure definitions for WAV file handling, common to IR ops handling and output
*
* Author: Josh McIntyre
*/

#pragma once

// Define a header struct for the Wav file format we can use to parse/modify the header
// Most will remain unchanged, other than the sample size
struct WavHeader{

    char riffMagicHeader[4];
    uint32_t chunkSize;
    char waveHeader[4];
    char fmtHeader[4];
    uint32_t fmtChunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t samplesPerSec;
    uint32_t bytesPerSec;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char dataString[4];
    uint32_t dataLength;

};