TEMPLATE = app
TARGET = FotoLab
QT += core \
    gui
HEADERS += Sources/gui/polygonitem.h \
    Sources/gui/imageview.h \
    Sources/gui/imageitem.h \
    Sources/gui/dialogsettings.h \
    Sources/fotolab/houghtransform.h \
    Sources/fotolab/houghline.h \
    Sources/fotolab/fotolabkgb.h \
    Sources/fotolab/cannyedgedetector.h \
    Sources/fotolab/fotolabbackend.h \
    Sources/gui/fotolab.h \
    Sources/kht_source/buffer_2d.h \
    Sources/kht_source/eigen.h \
    Sources/kht_source/kht.h \
    Sources/kht_source/linking.h \
    Soureces/kht_source/peak_detection.h \
    Sources/kht_source/subdivision.h \
    Sources/kht_source/voting.h
SOURCES += Sources/gui/polygonitem.cpp \
    Sources/gui/imageview.cpp \
    Sources/gui/imageitem.cpp \
    Sources/gui/dialogsettings.cpp \
    Sources/fotolab/houghtransform.cpp \
    Sources/fotolab/houghline.cpp \
    Sources/fotolab/fotolabkgb.cpp \
    Sources/fotolab/cannyedgedetector.cpp \
    Sources/fotolab/fotolabbackend.cpp \
    Sources/gui/fotolab.cpp \
    Sources/external/pgm_io.c \
    Sources/external/hysteresis.c \
    Sources/external/canny_edge.c \
    Sources/kht_source/buffer_2d.cpp \
    Sources/kht_source/eigen.cpp \
    Sources/kht_source/kht.cpp \
    Sources/kht_source/linking.cpp \
    Sources/kht_source/peak_detection.cpp \
    Sources/kht_source/subdivision.cpp \
    Sources/kht_source/voting.cpp \
    Sources/main.cpp
FORMS += Sources/fotolab.ui \
	Sources/gui/dialogsettings.ui
RESOURCES += Resources/Images/sample_images.qrc \
    Resources/Icons/icons.qrc
UI_DIR = Sources/Ui
TRANSLATIONS = FotoLab_sk.ts
