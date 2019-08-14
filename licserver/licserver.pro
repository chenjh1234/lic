
include (../licInclude.config)
TEMPLATE = lib
 
INCLUDEPATH +=  \
	../licconfig \
	../licbase 

  
DESTDIR =$${LIC_LIB}
TARGET = licserver
LIBS += \
	-L $${LIC_ROOT_PATH}/lib
	

 HEADERSSS +=  \
	   SLicData.h \
	   SAgent.h\
	   SLicMng.h 
 
 

 SOURCES +=  \
	   SLicMng.cpp \
	   SLicData.cpp \
#	   SCycleThread.cpp \  
	   SAgent.cpp
	   
 
 


# below is for install:  / is important we must install from /
APPLIC.path +=/$${LIC_ROOT_PATH}/file
APPLIC.files = ../apps/apps.cpp ../apps/apps.pro ../licwork/licfile.sample ../apps/licdev.pdf ../apps/licgen.pdf ../apps/stjob.pdf \
	../licwork/* ../testconfig/*.conf  ../client/applic.h
INSTALLS +=APPLIC

SH.path +=/$${LIC_ROOT_PATH}/bin
SH.files = ../sh/* 
INSTALLS +=SH
#message ("apppath= " $$APPLIC.path)

