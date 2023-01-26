TARGET = program.program
DESTDIR = ./files
QT += widgets
HEADERS += program.hpp processed_string.hpp extension.hpp my_filesystem.hpp data_retriever.hpp
SOURCES += main.cpp data_retriever.cpp
INCLUDEPATH += /usr/include/qt
QMAKE_CXXFLAGS += -std=c++20
