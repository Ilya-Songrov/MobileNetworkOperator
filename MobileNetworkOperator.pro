QT += quick sql widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        database/dbcreator.cpp \
        database/managerdb.cpp \
        main.cpp \
        mobilenetworkoperatorapplicationengine.cpp \
        providerQml/providerqml.cpp \
        treeview/customtype.cpp \
        treeview/treeitem.cpp \
        treeview/treemodel.cpp

HEADERS += \
        database/dbcreator.h \
        database/managerdb.h \
        mobilenetworkoperatorapplicationengine.h \
        providerQml/providerqml.h \
        treeview/customtype.h \
        treeview/itemdata.h \
        treeview/treeitem.h \
        treeview/treemodel.h

RESOURCES += \
        resources/qml.qrc \
        resources/images.qrc

INCLUDEPATH += \
        treeview \
        database \
        providerQml

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

