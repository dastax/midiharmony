TEMPLATE    = app

SOURCES     = jack_engine.cpp \
midiharmony.cpp \
HarmonyTrigger.cpp \
Harmony.cpp \
main.cpp

HEADERS     = jack_engine.h \
HarmonyTrigger.h \
Harmony.h \
midiharmony.h

INCLUDEPATH += /usr/include/

LIBS += -ljack -lqjack

QT += xml

harmonydb.path = /usr/share/midiharmony/
harmonydb.files = ./harmonydb.xml

target.path += /usr/bin/
INSTALLS += target
INSTALLS += harmonydb

RESOURCES= application.qrc

CONFIG += release

DESTDIR=bin
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
