This is my submission for the COMP5891M Foundations of Computer Science programming coursework. It has all tasks completed including the advanced tasks.

Main program
To compile:
qmake -project "QT+=core gui widgets opengl openglwidgets" "QMAKE_CXXFLAGS+= -fopenmp -Wall" "QMAKE_CXXFLAGS += -fopenmp -D_GLIBCXX_PARALLEL" "QMAKE_LFLAGS += -fopenmp" "LIBS += -lGL -lGLU -fopenmp"
qmake
make

 To run:
QT_QPA_PLATFORM=xcb ./BezierPatchWindowRelease ../input/patch.txt


Math library tests
To compile:
make

To run:
./testLibrary

Design decisions:
- The primary parallelised section of my rasteriser is the bezier patch, since the thread creation and synchronisation overhead would outweigh the speedup produced for basic lines and circles. However, the bezier patch has many more independent iterations, making it an ideal target for parallelisation.

- The number of samples in the bezier patch has been selected based on a balance between performance and visuals. Raising the sample count will improve consistency at high zoom levels, but will result in lower performance.