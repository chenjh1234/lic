include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = borrowreturn

INCLUDEPATH += $${INCLUDE_BUS}

INCLUDEPATH += $${INCLUDE_UTIL}

LIBS +=  $${LIBS_BUS}
LIBS += $${LIBS_APP}

SOURCES += \
	  borrowreturn.cpp 
SOURCES +=  $${SOURCE_APP}
