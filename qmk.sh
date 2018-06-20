echo ======= libs=======
echo  licbase
cd licbase
qmake
echo  licconfig
cd ../licconfig
qmake
echo  licserver
cd ../licserver
qmake
echo  liccommon
cd ../liccommon
qmake
echo  client
cd ../client
qmake
echo  server
cd ../server
qmake
echo httpcall
cd ../http
qmake
echo  draw
cd ../draw
qmake
echo  property
cd ../property
qmake
echo ======= apps-test =======
echo  licdev
cd ../licdev
qmake
echo  testlic
cd ../testlic
qmake
echo  tsetapi
cd ../testapi
qmake
echo =======utils =======
# util----------------------
echo  util
cd ../util
qmake
echo  loadfile
cd loadfile
qmake
echo  appt
cd ../appt
qmake
echo  report
cd ../report
qmake
echo  shutdownlilc
cd ../shutdownlic
qmake
echo  createlic
cd ../createlic
qmake
echo createkey
cd ../createkey
qmake
echo  uloadfile
cd ../unloadfile
qmake
echo unpack
cd ../unpack
qmake
echo  borrow
cd ../borrow
qmake
echo  borrowreturn
cd ../borrowreturn
qmake
echo  passwd2m
cd ../passwd2m
qmake
echo  passwdtoday
cd ../passwdtoday
qmake
echo showdb
cd ../showdb
qmake
echo showproof
cd ../showproof
qmake
echo  stjob
cd ../stjob
qmake
echo  licgen
cd ../licgen
qmake
echo  uuid
cd ../uuid
qmake
echo  mid
cd ../mid
qmake
echo viewconfig
cd ../viewconfig
qmake
echo loadconfig
cd ../loadconfig
qmake
echo clearconfig
cd ../clearconfig
qmake
#--------// end of util
echo ======= apps-test =======
echo apps
cd ../../apps
qmake
echo portal
cd ../portal
qmake


