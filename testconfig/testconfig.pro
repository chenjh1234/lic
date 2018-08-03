 
include (../licInclude.config)
TEMPLATE = app
 
INCLUDEPATH +=  \
	../licconfig\
	../licbase 
  
DESTDIR =$${LIC_BIN}
TARGET = testconfig

LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase -llicconfig
 
HEADERS +=  

SOURCES +=  \ 
            licconfig.cpp


