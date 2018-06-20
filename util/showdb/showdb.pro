include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = showdb

#INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_UTIL}
#LIBS +=  $${LIBS_BUS}
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicconfig -llicserver

SOURCES += \
	  showdb.cpp 
