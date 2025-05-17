# RT-AEP

Real-Time Audio Effects Processor (RT-AEP)

## Features

- Real-time audio streaming via portAudio
- DSP with multiple audio effects
- GUI for parameter control using Dear ImGui

## Technologies

- C++17
- **Audio Backend:** PortAudio
- **GUI:** Dear ImGui
- **Build System:** CMake

## Build Instructions

### Requirements

- C++17 compiler
- CMake ≥ 3.15
- PortAudio library

### PortAudio Setup

This project requires PortAudio. If CMake cannot find PortAudio automatically, set the `PORTAUDIO_PATH` environment variable to the PortAudio installation directory (containing `include` and `lib`).


### Build Steps (MSVC/Visual Studio)

```powershell
# Clean previous build
Remove-Item -Recurse -Force .\build\

# Configure with MSVC
cmake -S . -B build -G "Visual Studio 17 2022" -A x64

# Build in Release mode
cmake --build build --config Release
```
