TEMPLATE = app

QT += qml quick widgets

# for now, use included pre-protoc'ed includes and C++ files:
PROTOGEN = generated

NZMQTDIR = /home/alexander/projects/QtQuickVcp/qmlhalplugin/externals-src/nzmqt
ZEROMQ_ANDROID_DIR = /opt/zeromq-android
PROTOBUF_ANDROID_DIR = /home/alexander/projects/tmp/protobuf-read-only


# This define will "move" nzmqt class method
# implementations to nzmqt.cpp file.
DEFINES += NZMQT_LIB

SOURCES += main.cpp \
    $$PROTOGEN/frame.pb.cc \
    $$NZMQTDIR/src/nzmqt/nzmqt.cpp \
    mjpegstreamerclient.cpp

HEADERS += \
    $$PROTOGEN/frame.pb.h \
    $$NZMQTDIR/include/nzmqt/global.hpp \
    $$NZMQTDIR/include/nzmqt/nzmqt.hpp \
    $$NZMQTDIR/include/nzmqt/impl.hpp \
    mjpegstreamerclient.h

RESOURCES += qml.qrc

INCLUDEPATH += \
    $$NZMQTDIR/include \
    $$NZMQTDIR/externals/include \
    $$PROTOGEN

android: {
LIBS += -L$$ZEROMQ_ANDROID_DIR/lib/ \
        -L$$PROTOBUF_ANDROID_DIR/src/.libs/
INCLUDEPATH += $$ZEROMQ_ANDROID_DIR/include/ \
            $$PROTOBUF_ANDROID_DIR/src/
}

LIBS += -lzmq -lprotobuf

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
