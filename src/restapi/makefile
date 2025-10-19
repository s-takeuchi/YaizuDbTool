LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lstdc++fs -pthread -static-libgcc -lssl -lcrypto -lstdc++ -lodbc -lodbcinst
CCOPTS    = -DUNICODE -std=c++1z

cmdfreak : CmdFreakRestApi.o MyMsgProc.o dataaccess.o ApiGetSystem.o ApiGetRecCount.o ApiGetRecord.o ApiGetTableInfo.o ApiFilterInfo.o ApiOdbcInfo.o OdbcManager.o Global.o
	cd ../../../YaizuComLib/src/stkpl; make
	cp ../../../YaizuComLib/src/stkpl/stkpl.a .
	cd ../../../YaizuComLib/src/stkwebapp; make
	cp ../../../YaizuComLib/src/stkwebapp/stkwebapp.a .
	cd ../../../YaizuComLib/src/stkwebapp_um; make
	cp ../../../YaizuComLib/src/stkwebapp_um/stkwebapp_um.a .
	cd ../../../YaizuComLib/src/commonfunc; make
	cp ../../../YaizuComLib/src/commonfunc/commonfunc.a .
	cd ../../../YaizuComLib/src/stksocket; make
	cp ../../../YaizuComLib/src/stksocket/stksocket.a .
	cd ../../../YaizuComLib/src/stkthread; make
	cp ../../../YaizuComLib/src/stkthread/stkthread.a .
	cd ../../../YaizuComLib/src/stkdata; make
	cp ../../../YaizuComLib/src/stkdata/stkdata.a .
	cd ../../../YaizuComLib/src/stkdb; make
	cp ../../../YaizuComLib/src/stkdb/stkdb.a .

	g++ -o cmdfreak CmdFreakRestApi.o MyMsgProc.o dataaccess.o ApiGetSystem.o  ApiGetRecCount.o ApiGetRecord.o ApiGetTableInfo.o ApiFilterInfo.o ApiOdbcInfo.o OdbcManager.o Global.o stkpl.a stkwebapp.a stkwebapp_um.a commonfunc.a stksocket.a stkthread.a stkdata.a stkdb.a $(LDFLAGS) $(LIBS)

CmdFreakRestApi.o : CmdFreakRestApi.cpp
	g++ -c CmdFreakRestApi.cpp $(CCOPTS)

MyMsgProc.o : MyMsgProc.cpp
	g++ -c MyMsgProc.cpp $(CCOPTS)

dataaccess.o : dataaccess.cpp
	g++ -c dataaccess.cpp $(CCOPTS)

ApiGetSystem.o : ApiGetSystem.cpp
	g++ -c ApiGetSystem.cpp $(CCOPTS)

ApiGetRecCount.o : ApiGetRecCount.cpp
	g++ -c ApiGetRecCount.cpp $(CCOPTS)

ApiGetRecord.o : ApiGetRecord.cpp
	g++ -c ApiGetRecord.cpp $(CCOPTS)

ApiGetTableInfo.o : ApiGetTableInfo.cpp
	g++ -c ApiGetTableInfo.cpp $(CCOPTS)

ApiFilterInfo.o : ApiFilterInfo.cpp
	g++ -c ApiFilterInfo.cpp $(CCOPTS)

ApiOdbcInfo.o : ApiOdbcInfo.cpp
	g++ -c ApiOdbcInfo.cpp $(CCOPTS)

OdbcManager.o : OdbcManager.cpp
	g++ -c OdbcManager.cpp $(CCOPTS)

Global.o : Global.cpp
	g++ -c Global.cpp $(CCOPTS)


clean :
	rm -f *.o *.a cmdfreak
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/stkwebapp_um; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean
	cd ../../../YaizuComLib/src/stkdata; make clean
	cd ../../../YaizuComLib/src/stkdb; make clean

all : clean cmdfreak
