#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T10:42:19
#
#-------------------------------------------------

QT       += core gui opengl xml

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
    OpenGL/Shaders/GLShader.cpp \
    OpenGL/Shaders/SolidShader.cpp \
    SubdomainTools/CircleTool.cpp \
    Layers/SelectionLayer.cpp \
    Layers/Actions/Action.cpp \
    Layers/Actions/NodeAction.cpp \
    Layers/Actions/ElementAction.cpp \
    OpenGL/Shaders/GradientShader.cpp \
    Domains/Domain.cpp \
    OpenGL/Shaders/CulledSolidShader.cpp \
    Layers/SelectionLayers/CreationSelectionLayer.cpp \
    Layers/Actions/ElementState.cpp \
    SubdomainTools/BoundaryFinder.cpp \
    SubdomainTools/RectangleTool.cpp \
    SubdomainTools/PolygonTool.cpp \
    SubdomainTools/SelectionTool.cpp \
    Dialogs/CreateProjectDialog.cpp \
    Quadtree/SearchTools/PolygonSearch.cpp \
    Quadtree/Quadtree.cpp \
    Quadtree/SearchTools/CircleSearch.cpp \
    Quadtree/SearchTools/RectangleSearch.cpp \
    Quadtree/SearchTools/DepthSearch.cpp \
    Quadtree/SearchTools/ClickSearch.cpp \
    SubdomainTools/ClickTool.cpp \
    Projects/Project.cpp

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
    OpenGL/Shaders/GLShader.h \
    OpenGL/Shaders/SolidShader.h \
    SubdomainTools/CircleTool.h \
    Layers/SelectionLayer.h \
    Layers/Actions/Action.h \
    Layers/Actions/NodeAction.h \
    Layers/Actions/ElementAction.h \
    OpenGL/Shaders/GradientShader.h \
    Domains/Domain.h \
    OpenGL/Shaders/CulledSolidShader.h \
    Layers/SelectionLayers/CreationSelectionLayer.h \
    Layers/Actions/ElementState.h \
    SubdomainTools/BoundaryFinder.h \
    SubdomainTools/RectangleTool.h \
    SubdomainTools/PolygonTool.h \
    SubdomainTools/SelectionTool.h \
    Dialogs/CreateProjectDialog.h \
    Quadtree/SearchTools/PolygonSearch.h \
    Quadtree/Quadtree.h \
    Quadtree/QuadtreeData.h \
    Quadtree/SearchTools/CircleSearch.h \
    Quadtree/SearchTools/RectangleSearch.h \
    Quadtree/SearchTools/DepthSearch.h \
    Quadtree/SearchTools/ClickSearch.h \
    SubdomainTools/ClickTool.h \
    Projects/Project.h

FORMS    += MainWindow.ui \
    Dialogs/CreateProjectDialog.ui

target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += AdcircSubdomainTool.desktop

INSTALLS += target desktop

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    AdcircSubdomainTool.desktop
