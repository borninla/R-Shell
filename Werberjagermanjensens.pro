TEMPLATE += app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
	src/main.cpp \
	src/delim.cpp \
	src/manager.cpp \
	src/shunting_yard.cpp \
	src/token.cpp \
	src/util.cpp

HEADERS += \
	header/delim.h \
	header/manager.h \
	header/shunting_yard.h \
	header/token.h \
	header/util.h
	
