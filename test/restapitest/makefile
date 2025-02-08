LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lstdc++fs -pthread -static-libgcc -lssl -lcrypto -lstdc++ -lodbc -lodbcinst
CCOPTS    = -DUNICODE -std=c++1z

restapitest : restapitest.o 
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

	g++ -o restapitest restapitest.o stkpl.a stkwebapp.a stkwebapp_um.a commonfunc.a stksocket.a stkthread.a stkdata.a $(LDFLAGS) $(LIBS)

restapitest.o : restapitest.cpp
	g++ -c restapitest.cpp $(CCOPTS)

clean :
	rm -f *.o *.a restapitest
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/stkwebapp; make clean
	cd ../../../YaizuComLib/src/stkwebapp_um; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean
	cd ../../../YaizuComLib/src/stksocket; make clean
	cd ../../../YaizuComLib/src/stkthread; make clean
	cd ../../../YaizuComLib/src/stkdata; make clean

all : clean restapitest
