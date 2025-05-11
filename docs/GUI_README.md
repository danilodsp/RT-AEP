# RT-AEP GUI Integration

## Dependencies
- [Dear ImGui](https://github.com/ocornut/imgui) (with backends: imgui_impl_glfw.cpp, imgui_impl_opengl3.cpp)
- [GLFW](https://www.glfw.org/)
- OpenGL (system)

ImGui and GLFW sources should be in `external/` as follows:

```
external/
  imgui/
    imgui.h, imgui.cpp, ...
    backends/
      imgui_impl_glfw.cpp, imgui_impl_opengl3.cpp, ...
  glfw/
    CMakeLists.txt, src/, include/, ...
```

## Building

- Ensure PortAudio, ImGui, and GLFW are present.
- Set `PORTAUDIO_PATH` environment variable.
- Run CMake and build.

## Notes

- All effect parameters are thread-safe using `std::atomic`.
- No new threads are created; GUI runs on main thread, audio on PortAudio callback.
- For cross-platform OpenGL/GLFW/ImGui setup, see ImGui and GLFW documentation.
