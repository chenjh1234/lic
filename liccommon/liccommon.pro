
include (../licInclude.config)
TEMPLATE = lib
INCLUDEPATH +=  $${INCLUDE_BUS}

 
DESTDIR =$${LIC_LIB}
TARGET = liccommon
 
INCLUDEPATH  +=  $${LIC_SRC_PATH}/licbase

SOURCES += \
	   $${TETRIS_HOME}/src/lib/distributed_bus.cpp \
	   $${TETRIS_HOME}/src/lib/bus.cpp \   
	   jsonParser.cpp
 
 

