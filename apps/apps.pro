include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = apps

INCLUDEPATH += . 
 

#INCLUDEPATH += $${INCLUDE_BUS}
#INCLUDEPATH += $${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 

 
SOURCES += \
	   apps.cpp 
SOURCES +=  $${SOURCE_APP}
