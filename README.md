# Gaussian Blur (Normal Distribution)
Key Bindings:
- Esc - Exit program
- Space bar          - Toggle filter
- Mouse scroll wheel - Modify standard deviation of distribution

### TODO(s):
-  Implement as seperable filters to improve performance

# Reference(s):
- [Grant Sanderson | Convolutions in image processing](https://www.youtube.com/watch?v=8rrHTtUzyZA)
- [Wikipedia | Gaussian Blur](https://en.wikipedia.org/wiki/Gaussian_blur)

<hr>

***Platform build***: x86

***OpenGL***: GLSL4.3, OpenGL32.lib, glad

***Windowing***: GLFW

***Math library***: GLM

<hr>

## CMake build (With CMake installed and command prompt opened in project's root directory)

- Generate project files for x86 platform ( in this case win32 )
```
[Project root directory]: cmake . -B build -A Win32
```
- Build project
```
[Project root directory]: cmake --build build -t GaussianBlur
[Project root directory]: cd build/Debug
[Project root directory / build / Debug]: GaussianBlur
```

<hr>

![img](https://res.cloudinary.com/asuelimf/image/upload/v1636928558/ProjectScreenshots/MathsOpenGLGausianblur_r7wcml.png)
