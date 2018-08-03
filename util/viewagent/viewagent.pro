include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_ROOT_PATH}/bin
TARGET = viewagent


INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}
LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APP}

SOURCES +=  $${SOURCE_APP}

SOURCES += \
	   viewagent.cpp 
