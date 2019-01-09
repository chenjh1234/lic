include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = appf

INCLUDEPATH += . $${LIC_ROOT_PATH}/file ../client
 


LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 

 
SOURCES += \
	   appf.f
SOURCES +=  $${SOURCE_APP}
