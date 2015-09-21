SOURCES += main.cpp \
           MainWindowAGRenderer.cpp \
           imagenWidget.cpp \
           renderWidget.cpp \
 dlgConfigurarAG.cpp \
 threadAG.cpp \
 Cromosoma.cpp
TEMPLATE = app
CONFIG += warn_on \
          thread \
          qt \
          release \
          static 

TARGET = ag_renderer
DESTDIR = ../bin
QT += gui

unix : INCLUDEPATH += /usr/include/qt4/QtSvg/ 
win32:  INCLUDEPATH += D:/Development/Qt/include/QtSvg

#EL NOMBRE DE LAS LIBRERIAS NO INCLUYE EL LIB...
unix : LIBS += -lQtSvg

HEADERS += MainWindowAGRenderer.h \
           imagenWidget.h \
           renderWidget.h \
 dlgConfigurarAG.h \
 threadAG.h \
 Cromosoma.h
FORMS += ../Ui/dlgConfigurarAG.ui \
 ../Ui/MainWindowAGRenderer.ui

RESOURCES += ../Ui/cebolla.qrc

