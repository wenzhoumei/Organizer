TARGET = program.program
DESTDIR = ./files
QT += widgets
HEADERS += program.hpp processed_string.hpp processed_extension.hpp
SOURCES += main.cpp processed_extension.cpp
INCLUDEPATH += /usr/include/qt
QMAKE_CXXFLAGS += -std=c++20
