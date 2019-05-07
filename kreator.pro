QMAKE_CXX = "g++ -g"

unix {
    QMAKE_CXX = "colorgcc -g"
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

macx {
    QMAKE_CXX = "g++"
    UI_DIR = .ui
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    ICON = kreator.icns
    QMAKESPEC =macx-g++
}

TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
DEFINES += KREATOR_DEBUG
RESOURCES = tskreator.qrc
RC_FILE = kreator.rc

QT += widgets

FORMS += guiarea.ui \
         guichangevnumbers.ui \
         guicreatearea.ui \
         guiexit.ui \
         guiextradescription.ui \
         guifind.ui \
         guiitem.ui \
         guiitemaffects.ui \
         guiitemvalues.ui \
         guimainwindow.ui \
         guimob.ui \
         guimobinit.ui \
         guireport.ui \
         guiroom.ui \
         guiselectflags.ui \
         guisettings.ui \
         guishop.ui \
         guiteleportroom.ui \
	 guitester.ui \
         guizone.ui \
         guizonecommand.ui \
         guimultiplemodify.ui
HEADERS += area.h \
           bit.h \
           characterdata.h \
           config.h \
           constantname.h \
           constants.h \
           dice.h \
           eleuconf.h \
           exception.h \
           exit.h \
           extradescription.h \
           guiutils.h \
           guiversion.h \
           handler.h \
           item.h \
           itemaffect.h \
           kreatorsettings.h \
           tskreator.h \
           myqt.h \
           mob.h \
           mobinit.h \
           objectdata.h \
           random.h \
           report.h \
           room.h \
           selectobject.h \
           settings.h \
           shop.h \
           spell.h \
	   syntax.h \
           types.h \
           utils.h \
           validator.h \
           version.h \
           wndarea.h \
           wndchangevnumbers.h \
           wndcreatearea.h \
           wndexit.h \
           wndextradescription.h \
	   wndtester.h \
           wnditem.h \
           wnditemaffects.h \
           wnditemvalues.h \
           wndmainwindow.h \
           wndmob.h \
           wndmobinit.h \
           wndreport.h \
           wndroom.h \
           wndfind.h \
           wndselectflags.h \
           wndsettings.h \
           wndshop.h \
           wndteleportroom.h \
           wndzone.h \
           wndzonecommand.h \
           zone.h \
           zonecommand.h \
           zonecommandlist.h \
           zonecommandgroup.h \
           zoneinitlist.h \
 wndmultiplemodify.h
SOURCES += area.cpp \
           bit.cpp \
           characterdata.cpp \
           constantname.cpp \
           dice.cpp \
           eleuconf.cpp \
           exception.cpp \
           exit.cpp \
           extradescription.cpp \
           guiutils.cpp \
           handler.cpp \
           item.cpp \
           itemaffect.cpp \
           kreatorsettings.cpp \
           tskreator.cpp \
           main.cpp \
           myqt.cpp \
           mob.cpp \
           mobinit.cpp \
           objectdata.cpp \
           random.cpp \
           report.cpp \
           room.cpp \
           selectobject.cpp \
           settings.cpp \
           shop.cpp \
           spell.cpp \
	   syntax.cpp \
           utils.cpp \
           validator.cpp \
           wndarea.cpp \
           wndchangevnumbers.cpp \
           wndcreatearea.cpp \
           wndexit.cpp \
           wndextradescription.cpp \
           wndshop.cpp \
	   wndtester.cpp \
           wnditem.cpp \
           wnditemvalues.cpp \
           wnditemaffects.cpp \
           wndmainwindow.cpp \
           wndmob.cpp \
           wndmobinit.cpp \
           wndreport.cpp \
           wndroom.cpp \
           wndfind.cpp \
           wndselectflags.cpp \
           wndsettings.cpp \
           wndteleportroom.cpp \
           wndzone.cpp \
           wndzonecommand.cpp \
           zone.cpp \
           zonecommand.cpp \
           zonecommandlist.cpp \
           zonecommandgroup.cpp \
           zoneinitlist.cpp \
        wndmultiplemodify.cpp
