 
include (../licInclude.config)
TEMPLATE = lib
 
INCLUDEPATH +=  \
	../licbase 
  
DESTDIR =$${LIC_LIB}
TARGET = licconfig

LIBS += \
	-L $${LIC_ROOT_PATH}/lib
 
HEADERS +=  
 

SOURCES +=  \ 
            LimitData.cpp\
            LimitConfig.cpp\
            configMng.cpp


