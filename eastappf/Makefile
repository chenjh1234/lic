


F90 = f95
FORLINKER = f95

F90FLAGS =   -fPIC   -fbackslash -ff2c  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
#F90FLAGS = -zero -fPIC -w -assume 2underscores -assume bscc -assume byterecl -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
OPTFLAGS = -O2 -xW


LIBS          =  -least -lfc -luuid -L$(HOME)/tetris/src/third/openssl/lib -lcrypto -lssl -L$(HOME)/tetris/src/third/json/lib -ljsoncpp -L/mnt/hgfs/win/boost/lib -lboost_date_time -lboost_filesystem -lboost_system -lboost_thread -lboost_serialization -L $(HOME)/licsrc/install/lib -llicbase -llicclient -lliccommon -llichttpcall -lcurl -llicconfig /lib64/libdl.so.2 -L/home/cjh/qt561/lib -lQt5Gui -L/usr/lib64 -lQt5Core -lGL -lpthread 
 

OBJECTS_DIR   = ./
 

SOURCES       = eastappf.f 
OBJECTS       = eastappf.o
   
 
DESTDIR       = $(HOME)/licsrc/install/bin/
TARGET        = $(HOME)/licsrc/install/bin/eastappf
 
$(TARGET):  $(OBJECTS) 
	$(FORLINKER)  -o $(TARGET) $(OBJECTS)  $(LIBS)
  

####### Compile

 $(OBJECTS): $(SOURCES)
	$(F90) -c $(SOURCES) -o $(OBJECTS) $(F90FLAGS)  

 
