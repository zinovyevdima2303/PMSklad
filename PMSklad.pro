QT       += core gui sql
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_PSQL
TEMPLATE = app
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    login.cpp \
    mainuseradmin.cpp \
    mainuseradminadd.cpp \
    mainuseradminmodify.cpp \
    mainusersworkers.cpp \
    mainusersworkersinventory.cpp \
    mainusersworkersinventoryadd.cpp \
    mainusersworkersinventorymodify.cpp \
    mainusersworkersinventoryprint.cpp \
    mainusersworkersreceipt_documents.cpp \
    mainusersworkersreceipt_documentsadd.cpp \
    mainusersworkersreceipt_documentsmodify.cpp \
    mainusersworkersreturn.cpp \
    mainusersworkersreturnadd.cpp \
    mainusersworkersreturnmodify.cpp \
    mainusersworkerstransfer.cpp \
    mainusersworkerstransferadd.cpp \
    mainusersworkerstransfermodify.cpp

HEADERS += \
    login.h \
    mainuseradmin.h \
    mainuseradminadd.h \
    mainuseradminmodify.h \
    mainusersworkers.h \
    mainusersworkersinventory.h \
    mainusersworkersinventoryadd.h \
    mainusersworkersinventorymodify.h \
    mainusersworkersinventoryprint.h \
    mainusersworkersreceipt_documents.h \
    mainusersworkersreceipt_documentsadd.h \
    mainusersworkersreceipt_documentsmodify.h \
    mainusersworkersreturn.h \
    mainusersworkersreturnadd.h \
    mainusersworkersreturnmodify.h \
    mainusersworkerstransfer.h \
    mainusersworkerstransferadd.h \
    mainusersworkerstransfermodify.h

FORMS += \
    login.ui \
    mainuseradmin.ui \
    mainuseradminadd.ui \
    mainuseradminmodify.ui \
    mainusersworkers.ui \
    mainusersworkersinventory.ui \
    mainusersworkersinventoryadd.ui \
    mainusersworkersinventorymodify.ui \
    mainusersworkersinventoryprint.ui \
    mainusersworkersreceipt_documents.ui \
    mainusersworkersreceipt_documentsadd.ui \
    mainusersworkersreceipt_documentsmodify.ui \
    mainusersworkersreturn.ui \
    mainusersworkersreturnadd.ui \
    mainusersworkersreturnmodify.ui \
    mainusersworkerstransfer.ui \
    mainusersworkerstransferadd.ui \
    mainusersworkerstransfermodify.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/fon.jpg \
    img/logo.png

RESOURCES += \
    src.qrc
