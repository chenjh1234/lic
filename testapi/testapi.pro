
include (../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = testapi
 
INCLUDEPATH +=$${INCLUDE_UTIL}
INCLUDEPATH +=$${INCLUDE_SERVER}
 
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicconfig   -llicserver#  /lib64/libdl.so.2  
HEADERS +=  
 
SOURCES +=  \
#            testCrypt.cpp\
	    testapi.cpp 	 
# work in licwork DIR
