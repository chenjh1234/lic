include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = apps

INCLUDEPATH += . $${LIC_ROOT_PATH}/file ../client
 


LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 

 
SOURCES += \
	   apps.cpp 
SOURCES +=  $${SOURCE_APP}
