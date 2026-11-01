## General
____________

### Author
* Josh McIntyre

### Website
* jmcintyre.net

### Overview
* CabinetConverter is a tool for resampling WAV format IR (impulse response) files

## Development
________________

### Git Workflow
* development for bugfixes and new features

### Building
* make build
Build the utility
* make tests
Build unit tests
* make clean
Clean the build directory

### Features
* Load and parse an existing WAV format IR file for a particular sample rate (ex: 48000 Hz)
* Re-sample the audio to a configured sample rate (ex: 44100) using linear interpolation
* Write the final re-sampled WAV file
* This tool is designed for common, mono WAV formats like those typically used for guitar/bass speaker cabinet IR files, but could be used for other IRs that meet the expected format

### Requirements
* Requires a terminal emulator
* Supply your own IRs, no samples are included with this project

### Platforms
* Windows
* Linux
* MacOSX

## Usage
____________

### CLI Usage
* Run `cabinetconverter.exe <input_filename> <output_filename> <convert_to_hz>`
* Program will parse the existing input WAV, and write the resampled WAV to output_filename
* Specify convert_to_hz in Hz, not KHz. For example, 44100 or 48000

### Unit Tests
* Run `test_cabinetconverter.exe`
