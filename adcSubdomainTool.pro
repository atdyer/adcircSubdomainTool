#-------------------------------------------------
#
# Project created by QtCreator 2013-06-10T10:42:19
#
#-------------------------------------------------

QT       += core gui opengl xml

LIBS += -lGLEW -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = adcSubdomainTool
TEMPLATE = app

QMAKE_CXXFLAGS += -O2


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
    Projects/Project.cpp \
    Projects/ProjectFile.cpp \
    Dialogs/DisplayOptionsDialog.cpp \
    Widgets/ShaderOptionsStackedWidget.cpp \
    Widgets/ColorWidgets/ColorGradientFrame.cpp \
    Widgets/ColorWidgets/ValueSlider.cpp \
    Widgets/ColorWidgets/BasicColorsWidget.cpp \
    Widgets/ColorWidgets/ColorButton.cpp \
    Widgets/ColorWidgets/CustomColorsWidget.cpp \
    Widgets/ColorWidgets/TriangleSliderButton.cpp \
    Widgets/ColorWidgets/GradientSliderFrame.cpp \
    Widgets/ColorWidgets/GradientSliderWidget.cpp \
    Widgets/ColorWidgets/SliderItemDelegate.cpp \
    Projects/IO/SubdomainCreator.cpp \
    Projects/ProjectSettings.cpp \
    Dialogs/ProjectSettingsDialog.cpp \
    Adcirc/FullDomainRunner.cpp \
    Dialogs/FullDomainRunOptionsDialog.cpp \
    Projects/IO/FileIO/Fort015.cpp \
    Projects/IO/FileIO/Fort066.cpp \
    Projects/IO/FileIO/Py140.cpp \
    Projects/IO/FileIO/Py141.cpp \
    Projects/IO/FileIO/Fort020.cpp \
    Projects/IO/FileIO/BNList14.cpp \
    NewProjectModel/Domains/FullDomain.cpp \
    NewProjectModel/Domains/SubDomain.cpp \
    NewProjectModel/Project_new.cpp \
    NewProjectModel/Files/Maxvel63_new.cpp \
    NewProjectModel/Files/Maxele63_new.cpp \
    NewProjectModel/Files/Fort64_new.cpp \
    NewProjectModel/Files/Fort63_new.cpp \
    NewProjectModel/Files/Py141_new.cpp \
    NewProjectModel/Files/Py140_new.cpp \
    NewProjectModel/Files/Fort22_new.cpp \
    NewProjectModel/Files/Fort021_new.cpp \
    NewProjectModel/Files/Fort15_new.cpp \
    NewProjectModel/ProjectSettings_new.cpp \
    NewProjectModel/Files/ProjectFile_new.cpp \
    NewProjectModel/Files/Fort14_new.cpp \
    NewProjectModel/Files/Fort020_new.cpp \
    NewProjectModel/Files/BNList14_new.cpp \
    NewProjectModel/Files/Fort022_new.cpp \
    NewProjectModel/Files/Fort015_new.cpp \
    NewProjectModel/Files/Fort066_new.cpp \
    NewProjectModel/Files/Fort067_new.cpp \
    Adcirc/SubdomainCreator_new.cpp \
    Adcirc/SubdomainRunner.cpp \
    SubdomainTools/BoundarySearch.cpp \
    NewProjectModel/Files/Workers/Fort14Reader.cpp

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
    Projects/Project.h \
    Projects/ProjectFile.h \
    Dialogs/DisplayOptionsDialog.h \
    Widgets/ShaderOptionsStackedWidget.h \
    Widgets/ColorGradientFrame.h \
    Widgets/ColorWidgets/ColorGradientFrame.h \
    Widgets/ColorWidgets/ValueSlider.h \
    Widgets/ColorWidgets/BasicColorsWidget.h \
    Widgets/ColorWidgets/ColorButton.h \
    Widgets/ColorWidgets/CustomColorsWidget.h \
    Widgets/ColorWidgets/TriangleSliderButton.h \
    Widgets/ColorWidgets/GradientSliderFrame.h \
    Widgets/ColorWidgets/GradientSliderWidget.h \
    Widgets/ColorWidgets/SliderItemDelegate.h \
    Projects/IO/SubdomainCreator.h \
    Projects/ProjectSettings.h \
    Dialogs/ProjectSettingsDialog.h \
    Adcirc/FullDomainRunner.h \
    Dialogs/FullDomainRunOptionsDialog.h \
    Projects/IO/FileIO/Fort015.h \
    Projects/IO/FileIO/Fort066.h \
    Projects/IO/FileIO/Py140.h \
    Projects/IO/FileIO/Py141.h \
    Projects/IO/FileIO/Fort020.h \
    Projects/IO/FileIO/BNList14.h \
    NewProjectModel/Domains/FullDomain.h \
    NewProjectModel/Domains/SubDomain.h \
    NewProjectModel/Project_new.h \
    NewProjectModel/Files/Maxvel63_new.h \
    NewProjectModel/Files/Maxele63_new.h \
    NewProjectModel/Files/Fort64_new.h \
    NewProjectModel/Files/Fort63_new.h \
    NewProjectModel/Files/Py141_new.h \
    NewProjectModel/Files/Py140_new.h \
    NewProjectModel/Files/Fort22_new.h \
    NewProjectModel/Files/Fort021_new.h \
    NewProjectModel/Files/Fort15_new.h \
    NewProjectModel/ProjectSettings_new.h \
    NewProjectModel/Files/ProjectFile_new.h \
    NewProjectModel/Files/Fort14_new.h \
    NewProjectModel/Files/Fort020_new.h \
    NewProjectModel/Files/BNList14_new.h \
    NewProjectModel/Files/Fort022_new.h \
    NewProjectModel/Files/Fort015_new.h \
    NewProjectModel/Files/Fort066_new.h \
    NewProjectModel/Files/Fort067_new.h \
    Adcirc/SubdomainCreator_new.h \
    Adcirc/SubdomainRunner.h \
    SubdomainTools/BoundarySearch.h \
    NewProjectModel/Files/Workers/Fort14Reader.h

FORMS    += MainWindow.ui \
    Dialogs/CreateProjectDialog.ui \
    Dialogs/DisplayOptionsDialog.ui \
    Widgets/ShaderOptionsStackedWidget.ui \
    Dialogs/ProjectSettingsDialog.ui \
    Dialogs/FullDomainRunOptionsDialog.ui

target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += AdcircSubdomainTool.desktop

INSTALLS += target desktop

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    AdcircSubdomainTool.desktop
