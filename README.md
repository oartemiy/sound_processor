# Sound Processor v1.0.1

A command‑line tool for processing and generating WAV audio files. It supports a chain of filters (amplify, normalize, insert silence, timestretch, lowpass) and generators (sine, AM, FM). The tool reads a WAV file (or uses a generator to create a signal), applies a series of transformations, and writes the result to an output file.

## Features

- **Filters** – modify an existing signal:
  - `ampl` – multiply amplitude by a factor (with clamping).
  - `normalize` – scale the signal so the maximum peak reaches a given level.
  - `silence` – insert silence (zero samples) at a specified interval; the rest of the signal shifts right.
  - `timestretch` – change duration by a factor using linear interpolation.
  - `lowpass` – smooth the signal with a moving‑average window of odd size (border samples are repeated).
- **Generators** – create a new signal from scratch:
  - `sin` – pure sine wave.
  - `am` – amplitude‑modulated sine wave.
  - `fm` – frequency‑modulated sine wave.
- **Multi‑channel support** – all filters (excepting generators) handle mono, stereo, and more channels correctly (each channel is processed independently).

## Download

Pre-built binaries are available on the [Releases](https://github.com/oartemiy/sound_processor/releases) page.

- **Linux**: `sound_processor-linux`
- **macOS**: `sound_processor-macos`

Make the file executable:

```bash
chmod +x sound_processor-linux
xattr -d com.apple.quarantine sound_processor-macos  # macOS
./sound_processor-linux -i input.wav -o output.wav -f ampl 1.5
```

## Building

### Prerequisites

- C++20 compiler (GCC, Clang, MSVC)
- CMake 3.24+
- [Ninja](https://ninja-build.org) 1.10+ (or other build system, like: [GNU make](https://www.gnu.org/software/make/manual/make.html) and so on)
- [vcpkg](https://github.com/microsoft/vcpkg) (for deps)
- Enviroment variables VCPKG_ROOT and VCPKG_TRIPLET  
- [Catch2](https://github.com/catchorg/Catch2) (for tests)

## Installing prerequisites

### vcpkg

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh      # on Linux/macOS
bootstrap-vcpkg.bat       # on Windows
```

### Enviroment variables VCPKG_ROOT and VCPKG_TRIPLET

```bash
export VCPKG_ROOT=/custom/path/to/vcpkg   # Linux/macOS
set VCPKG_ROOT=C:\custom\path\to\vcpkg   # Windows (cmd)
$env:VCPKG_ROOT = "C:\custom\path\to\vcpkg" # PowerShell
export VCPKG_TRIPLET=arm64-osx  # check your OS
```

### ... or in CMakePresets.json

```json
"environment": {
  "VCPKG_ROOT": "~/vcpkg",
  "VCPKG_TRIPLET": "arm64-osx"
},
```

### Ninja build. Or use other you want

```bash
sudo apt-get install -y ninja-build  # Linux
brew install ninja  # macOS
```

### Build steps

```bash
git clone https://github.com/oartemiy/sound_processor
cd sound_processor
cmake --preset debug  # or release
cmake --build --preset debug  # or release
./out/build/debug/sound_processor
```
