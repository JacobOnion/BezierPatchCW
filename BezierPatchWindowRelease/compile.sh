qmake -project "QT+=core gui widgets opengl openglwidgets" "QMAKE_CXXFLAGS+= -fopenmp -Wall" "QMAKE_CXXFLAGS += -fopenmp -D_GLIBCXX_PARALLEL" "QMAKE_LFLAGS += -fopenmp" "LIBS += -lGL -lGLU -fopenmp"
qmake
make
