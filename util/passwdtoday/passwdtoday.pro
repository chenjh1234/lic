include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = passwdtoday

 

INCLUDEPATH += $${INCLUDE_UTIL}
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicconfig -llicserver

SOURCES += \
	 passwdtoday.cpp 
