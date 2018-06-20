include (../../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = mid


#INCLUDEPATH += $${INCLUDE_BUS}

INCLUDEPATH +=./  \
	../../licbase
#LIBS +=  $${LIBS_BUS}
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase   

SOURCES += \
	  mid.cpp 

#SH.path =$${LIC_ROOT_PATH}/bin
#SH.files = ../sh/*.sh

#INSTALLS +=  SH



