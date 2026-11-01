/* This file contains code for reading and parsing a wav file, resampling, and writing
*
* Author: Josh McIntyre
*/
#include "irops.h"

// This function will read and parse a standard WAV file directly into memory
void read_wav(std::string input_filename, WavHeader &wav_header, std::vector<float> &wav_samples)
{
    // Open the binary wav file
    std::ifstream wav_file(input_filename, std::ios::binary);

    // Read the individual data fields of the header into a WavHeader struct instance
    // This will allow for more robust logic if necessary, but we're assuming a fixed format here
    // from a few known good IRs
    wav_file.read(wav_header.riffMagicHeader, 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.chunkSize), 4);
    wav_file.read(wav_header.waveHeader, 4);
    wav_file.read(wav_header.fmtHeader, 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.fmtChunkSize), 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.audioFormat), 2);
    wav_file.read(reinterpret_cast<char*>(&wav_header.numChannels), 2);
    wav_file.read(reinterpret_cast<char*>(&wav_header.samplesPerSec), 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.bytesPerSec), 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.blockAlign), 2);
    wav_file.read(reinterpret_cast<char*>(&wav_header.bitsPerSample), 2);
    wav_file.read(wav_header.dataString, 4);
    wav_file.read(reinterpret_cast<char*>(&wav_header.dataLength), 4);

    // Since we're working with IRs, assuming mono and reading sample by sample into floats
    int samples_length = wav_header.dataLength / (wav_header.bitsPerSample / 8);
    int bytes_per_sample = wav_header.bitsPerSample / 8;
    wav_samples.reserve(samples_length);

    // Currently, just support 24, 16, 32 bit sample sizes
    if (wav_header.bitsPerSample == 24)
    {
        for (int i = 0; i < samples_length; i++)
        {
            /* 24 bit requires a bit of special logic to fit properly into a 32-bit float
            * First, we'll need to bit-shift each byte to fit in the proper place in a 32-bit uint_32t
            * As well, we need to sign extend, since we're fitting a smaller integer in a larger placeholder
            * This will ensure the sign bit (ie: positive/negative) is in the right slot
            */
            unsigned char* bytes = new unsigned char[bytes_per_sample];

            wav_file.read(reinterpret_cast<char*>(bytes), bytes_per_sample);
            int32_t sample_int = bytes[0] | bytes[1] << 8 | bytes[2] << 16;
            if (sample_int & 0x800000)
            {
                sample_int |= ~0xFFFFFF;
            }

            float sample_float = static_cast<float>(sample_int);
            wav_samples.push_back(sample_float);

            delete bytes;
        }
    }
    else if (wav_header.bitsPerSample == 16 || wav_header.bitsPerSample == 32)
    {
        // TODO: Implement logic for 16/32 bit
        throw UNSUPPORTED_BIT_DEPTH_EXCEPTION;
    }
    else
    {
        throw UNSUPPORTED_BIT_DEPTH_EXCEPTION;
    }
}

// This function takes the finalized WAV format and writes it out to file
void write_wav(std::string output_filename, WavHeader &wav_header, std::vector<float> &wav_samples)
{
    // Open the binary wav file
    std::ofstream wav_file(output_filename, std::ios::binary);

    // Write the individual data fields of the header into the file using known sizes of each field
    wav_file.write(wav_header.riffMagicHeader, 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.chunkSize), 4);
    wav_file.write(wav_header.waveHeader, 4);
    wav_file.write(wav_header.fmtHeader, 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.fmtChunkSize), 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.audioFormat), 2);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.numChannels), 2);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.samplesPerSec), 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.bytesPerSec), 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.blockAlign), 2);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.bitsPerSample), 2);
    wav_file.write(wav_header.dataString, 4);
    wav_file.write(reinterpret_cast<const char*>(&wav_header.dataLength), 4);

    // Since we're working with IRs, assuming mono and reading sample by sample into floats
    int samples_length = wav_header.dataLength / (wav_header.bitsPerSample / 8);
    int bytes_per_sample = wav_header.bitsPerSample / 8;

    // Currently, just support 24, 16, 32 bit sample sizes
    if (wav_header.bitsPerSample == 24)
    {
        for (int i = 0; i < samples_length; i++)
        {
            /* Again, 24 bit requires some special handling
            * We'll cast from float into a uint32_t
            * Then, use bit shifting and masking to compile each byte into the correct slot in a char array (of size 3)
            */
            uint32_t sample_int = static_cast<int32_t>(std::round(wav_samples[i]));
            char* bytes = new char[bytes_per_sample];
            bytes[0] = sample_int & 0xFF;
            bytes[1] = (sample_int >> 8) & 0xFF;
            bytes[2] = (sample_int >> 16) & 0xFF;

            wav_file.write(bytes, bytes_per_sample);

            delete bytes;
        }
    }
    else if (wav_header.bitsPerSample == 16 || wav_header.bitsPerSample == 32)
    {
        // TODO: Implement logic for 16/32 bit
        throw UNSUPPORTED_BIT_DEPTH_EXCEPTION;
    }
    else
    {
        throw UNSUPPORTED_BIT_DEPTH_EXCEPTION;
    }
}

// This function handles the core re-sampling logic
// It will update the provided WavHeader with the appropriate metadata changes
// It will fill in the new sample vector with the interpolated values and return
std::vector<float> resample(int convert_to_hz, WavHeader &wav_header, std::vector<float> &original_samples)
{
    int resample_length = std::round(original_samples.size() * (float(convert_to_hz) / float(wav_header.samplesPerSec)));
    int original_hz = wav_header.samplesPerSec;

    std::vector<float> re_samples;
    re_samples.reserve(resample_length);

    /* Iterate over each new index in the output vector
    * Calculate the position that maps to in the input vector
    * This will often be a float rather than whole number, ex: 2.75
    * We'll need the "left neighbor" index (ex: 2), the "right neighbor" (ex: 3)
    * And the fraction/decimal portion (ex: 0.75)
    * If the right neighbor calculation is >= the size of the input vector, use the last index available (size - 1),
    * Otherwise, round up. The left neighbor is the rounded down value of the interpolated position
    * The final interpolated value goes in slot "i" in the re sampled vector
    * The linear interpolation formula is:
    *   interpolated = original_left + ((original_right - original_left) * fraction)
    */
    for (int i = 0; i < resample_length; i++)
    {

        float input_i = i * (float(original_hz) / float(convert_to_hz));
        int left_neighbor = std::floor(input_i);
        int right_neighbor = (std::ceil(input_i) >= original_samples.size()) ? (original_samples.size() - 1) : std::ceil(input_i);
        float fraction = input_i - left_neighbor;

        float original_left = original_samples[left_neighbor];
        float original_right = original_samples[right_neighbor]; 
        float interpolated = original_left + ((original_right - original_left) * fraction);

        re_samples.push_back(interpolated);
    }

    // After interpolating the data section, update the Wav Header with the new values that match the resampling
    // This is an argument return that updates the referenced wav_header
   wav_header.samplesPerSec = uint32_t(convert_to_hz);
   wav_header.dataLength = uint32_t(resample_length * wav_header.blockAlign);
   wav_header.bytesPerSec = uint32_t(convert_to_hz * wav_header.blockAlign);
   wav_header.chunkSize = uint32_t(RIFF_MAGIC_NUMBER + (resample_length * wav_header.blockAlign));

   return re_samples;

}

// This wrapper function handles the overall resampling operation
// It will call the appropriate functions to read/parse the input file, resample, and write the final file out
void wav_convert(std::string input_filename, std::string output_filename, int convert_to_hz)
{
    // Read and parse the input WAV file
    WavHeader input_wav_header{};
    std::vector<float> input_wav_samples;
    try
    {
        read_wav(input_filename, input_wav_header, input_wav_samples);   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    output_wavfile_info(input_filename, FILE_OP_INPUT, input_wav_header, input_wav_samples);

    // Resample the Wav
    WavHeader output_wav_header = input_wav_header;
    std::vector<float> output_wav_samples = resample(convert_to_hz, output_wav_header, input_wav_samples);

    // Write the output WAV file
    try
    {
        write_wav(output_filename, output_wav_header, output_wav_samples);   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    output_wavfile_info(output_filename, FILE_OP_OUTPUT, output_wav_header, output_wav_samples);

}