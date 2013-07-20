#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T10:42:19
#
#-------------------------------------------------

QT       += core gui opengl

include ( /usr/local/qwt-6.1.0/features/qwt.prf )

LIBS += -lGLEW -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = adcSubdomainTool
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    OpenGL/OpenGLPanel.cpp \
    OpenGL/glew.c \
    OpenGL/GLCamera.cpp \
    Layers/Layer.cpp \
    Layers/TerrainLayer.cpp \
    Layers/Quadtree.cpp \
    OpenGL/Shaders/GLShader.cpp \
    OpenGL/Shaders/SolidShader.cpp \
    SubdomainTools/CircleTool.cpp \
    Layers/SelectionLayer.cpp \
    Layers/Actions/Action.cpp \
    Layers/Actions/NodeAction.cpp \
    Layers/Actions/ElementAction.cpp \
    OpenGL/Shaders/GradientShader.cpp \
    Domains/Domain.cpp \
    OpenGL/Shaders/CulledSolidShader.cpp

HEADERS  += MainWindow.h \
    OpenGL/OpenGLPanel.h \
    OpenGL/wglew.h \
    OpenGL/glxew.h \
    OpenGL/glew.h \
    OpenGL/GLCamera.h \
    OpenGL/GLData.h \
    adcData.h \
    Layers/Layer.h \
    Layers/TerrainLayer.h \
    Layers/Quadtree.h \
    OpenGL/Shaders/GLShader.h \
    OpenGL/Shaders/SolidShader.h \
    SubdomainTools/CircleTool.h \
    Layers/SelectionLayer.h \
    Layers/Actions/Action.h \
    Layers/Actions/NodeAction.h \
    Layers/Actions/ElementAction.h \
    OpenGL/Shaders/GradientShader.h \
    Domains/Domain.h \
    OpenGL/Shaders/CulledSolidShader.h

FORMS    += MainWindow.ui

RESOURCES += \
    icons.qrc
