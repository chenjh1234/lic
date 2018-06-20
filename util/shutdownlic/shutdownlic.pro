
include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = shutdownlic

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH +=$${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP}

SOURCES += \
	  shutdownlic.cpp 
SOURCES +=  $${SOURCE_APP}
