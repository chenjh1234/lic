include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = createkey

#INCLUDEPATH += $${INCLUDE_BUS}

INCLUDEPATH +=./  \
	../../licbase
#LIBS +=  $${LIBS_BUS}
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase   

SOURCES += \
	  createkey.cpp 
