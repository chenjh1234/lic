include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = eastapp

INCLUDEPATH += . $${LIC_ROOT_PATH}/file ../client ../east
 


LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} -least

 
SOURCES += \
	   eastapp.cpp 
SOURCES +=  $${SOURCE_APP}
