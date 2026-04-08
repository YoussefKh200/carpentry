QT += core gui widgets charts sql printsupport
CONFIG += c++17
TEMPLATE = app
TARGET = fullpi

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    widgets/sidebarwidget.cpp \
    pages/basegestionpage.cpp \
    pages/loginpage.cpp \
    pages/settingspage.cpp \
    pages/userspage.cpp \
    pages/clientspage.cpp \
    pages/commandespage.cpp \
    pages/fournisseurspage.cpp \
    pages/materiauxpage.cpp \
    db/connection.cpp \
    models/usercrud.cpp \
    models/clientcrud.cpp \
    models/commandecrud.cpp \
    models/fournisseurcrud.cpp \
    models/materiauxcrud.cpp

HEADERS += \
    mainwindow.h \
    widgets/sidebarwidget.h \
    pages/basegestionpage.h \
    pages/loginpage.h \
    pages/settingspage.h \
    pages/userspage.h \
    pages/clientspage.h \
    pages/commandespage.h \
    pages/fournisseurspage.h \
    pages/materiauxpage.h \
    db/connection.h \
    models/usercrud.h \
    models/clientcrud.h \
    models/commandecrud.h \
    models/fournisseurcrud.h \
    models/materiauxcrud.h

FORMS += \
    mainwindow.ui \
    pages/loginpageform.ui \
    pages/settingspageform.ui \
    pages/userspageform.ui \
    pages/clientspageform.ui \
    pages/commandespageform.ui \
    pages/fournisseurspageform.ui \
    pages/materiauxpageform.ui

RESOURCES += \
    resources.qrc
