
include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = appt

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS +=   $${LIBS_APP}

SOURCES +=  $${SOURCE_APP} 
SOURCES += \
	   appt.cpp 
