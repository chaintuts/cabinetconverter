/* This file contains code for simple output functionality for dumping WAV file information
*
* Author: Josh McIntyre
*/
#include "opsui.h"

// This function outputs WAV file information with the filename context
void output_wavfile_info(std::string filename, std::string file_op, WavHeader &wav_header, std::vector<float> &wav_samples)
{
    std::cout << file_op << " file " << filename << " data:" << std::endl;

    std::cout << "riffMagicHeader: " << std::string_view(wav_header.riffMagicHeader, 4) << std::endl;
    std::cout << "chunksize: " << wav_header.chunkSize << std::endl;
    std::cout << "waveHeader: " << std::string_view(wav_header.waveHeader, 4) << std::endl;
    std::cout << "fmtHeader: " << std::string_view(wav_header.fmtHeader, 4) << std::endl;
    std::cout << "fmtChunkSize: " << wav_header.fmtChunkSize << std::endl;
    std::cout << "audioFormat: " << wav_header.audioFormat << std::endl;
    std::cout << "numChannels: " << wav_header.numChannels << std::endl;
    std::cout << "samplesPerSec: " << wav_header.samplesPerSec << std::endl;
    std::cout << "bytesPerSec: " << wav_header.bytesPerSec << std::endl;
    std::cout << "blockAlign: " << wav_header.blockAlign << std::endl;
    std::cout << "bitsPerSample: " << wav_header.bitsPerSample << std::endl;
    std::cout << "dataString: " << std::string_view(wav_header.dataString, 4) << std::endl;
    std::cout << "dataLength: " << wav_header.dataLength << std::endl;

    std::cout<< "Total parsed samples: " << wav_samples.size() << std::endl;
}