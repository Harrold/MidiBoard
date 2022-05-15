QT       += core gui multimedia xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    libremidi.cpp \
    midithread.cpp \
    profile.cpp \
    reader.cpp \
    sound.cpp \
    tablemodel.cpp \
    writer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    libremidi/api.hpp \
    libremidi/detail/alsa.hpp \
    libremidi/detail/coreaudio.hpp \
    libremidi/detail/dummy.hpp \
    libremidi/detail/emscripten.hpp \
    libremidi/detail/emscripten_api.hpp \
    libremidi/detail/jack.hpp \
    libremidi/detail/midi_api.hpp \
    libremidi/detail/midi_queue.hpp \
    libremidi/detail/raw_alsa.hpp \
    libremidi/detail/raw_alsa_helpers.hpp \
    libremidi/detail/semaphore.hpp \
    libremidi/detail/winmm.hpp \
    libremidi/detail/winuwp.hpp \
    libremidi/libremidi.hpp \
    libremidi/message.hpp \
    libremidi/reader.hpp \
    libremidi/writer.hpp \
    mainwindow.h \
    midithread.h \
    profile.h \
    sound.h \
    tablemodel.h

FORMS += \
    mainwindow.ui

DEFINES += LIBREMIDI_WINMM=1

LIBS += -lwinmm

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
