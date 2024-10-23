LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lssl -lcrypto -lstdc++ -lodbc -lodbcinst

sample : CmdFreakRestApi.o MyMsgProc.o dataaccess.o ApiGetSystem.o ApiGetRecCount.o ApiGetRecord.o ApiGetTableInfo.o ApiFilterInfo.o ApiOdbcInfo.o DbAccessor.o DbMariaDbAccessor.o DbMySqlAccessor.o DbPostgreSqlAccessor.o OdbcManager.o
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

	g++ -o sample CmdFreakRestApi.o MyMsgProc.o dataaccess.o ApiGetSystem.o  ApiGetRecCount.o ApiGetRecord.o ApiGetTableInfo.o ApiFilterInfo.o ApiOdbcInfo.o DbAccessor.o DbMariaDbAccessor.o DbMySqlAccessor.o DbPostgreSqlAccessor.o OdbcManager.o stkpl.a stkwebapp.a stkwebapp_um.a commonfunc.a stksocket.a stkthread.a stkdata.a -std=c++1z -lstdc++fs -pthread -static-libgcc $(LDFLAGS) $(LIBS)

CmdFreakRestApi.o : CmdFreakRestApi.cpp
	g++ -c CmdFreakRestApi.cpp -std=c++1z

MyMsgProc.o : MyMsgProc.cpp
	g++ -c MyMsgProc.cpp -std=c++1z

dataaccess.o : dataaccess.cpp
	g++ -c dataaccess.cpp -std=c++1z

ApiGetSystem.o : ApiGetSystem.cpp
	g++ -c ApiGetSystem.cpp -std=c++1z

ApiGetRecCount.o : ApiGetRecCount.cpp
	g++ -c ApiGetRecCount.cpp -std=c++1z

ApiGetRecord.o : ApiGetRecord.cpp
	g++ -c ApiGetRecord.cpp -std=c++1z

ApiGetTableInfo.o : ApiGetTableInfo.cpp
	g++ -c ApiGetTableInfo.cpp -std=c++1z

ApiFilterInfo.o : ApiFilterInfo.cpp
	g++ -c ApiFilterInfo.cpp -std=c++1z

ApiOdbcInfo.o : ApiOdbcInfo.cpp
	g++ -c ApiOdbcInfo.cpp -std=c++1z

DbAccessor.o : DbAccessor.cpp
	g++ -c DbAccessor.cpp -std=c++1z

DbMariaDbAccessor.o : DbMariaDbAccessor.cpp
	g++ -c DbMariaDbAccessor.cpp -std=c++1z

DbMySqlAccessor.o : DbMySqlAccessor.cpp
	g++ -c DbMySqlAccessor.cpp -std=c++1z

DbPostgreSqlAccessor.o : DbPostgreSqlAccessor.cpp
	g++ -c DbPostgreSqlAccessor.cpp -std=c++1z

OdbcManager.o : OdbcManager.cpp
	g++ -c OdbcManager.cpp -std=c++1z

clean :
	rm -f *.o *.a sample
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/stkwebapp_um; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean
	cd ../../../YaizuComLib/src/stkdata; make clean

all : clean sample
