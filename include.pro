LIC_ROOT_PATH = $${PWD}/install
LIC_SRC_PATH = $${PWD}
INCLUDE_BUS =   /mnt/hgfs/win/boost/include \
	/home/cjh/tetris/tetris/src/third/json/include\
	/home/cjh/tetris/tetris/src/third/openssl/include\
	/home/cjh/tetris/tetris/src/lib\
	/home/cjh/tetris/tetris/src/devel
LIBS_BUS = \
	-luuid\
	/home/cjh/tetris/tetris/src/third/openssl/lib/libcrypto.a \
	/home/cjh/tetris/tetris/src/third/openssl/lib/libssl.a \
	-L/home/cjh/tetris/tetris/src/third/json/lib -ljsoncpp\
	-L/mnt/hgfs/win/boost/lib -lboost_date_time -lboost_filesystem -lboost_system -lboost_thread -lboost_serialization 

