include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = passwd2m

INCLUDEPATH += $${INCLUDE_UTIL}

LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicconfig -llicserver

SOURCES += \
	  passwd2m.cpp 
