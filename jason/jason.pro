include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = jsstr

INCLUDEPATH += . $${INCLUDE_BUS} 

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 

 
SOURCES += \
	   jason.cpp 
SOURCES +=  $${SOURCE_APP}
