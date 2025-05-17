# Visualization Module

This maintains a circular buffer of recent audio frames.

- `Visualizer.hpp` / `Visualizer.cpp`: Core implementation.
- Usage: Call `pushAudio` from the audio thread, and `getWaveform` from the GUI thread.
