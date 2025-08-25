LDFLAGS = -L/usr/local/lib64 -L/usr/local/lib
LIBS    = -lstdc++fs -pthread -static-libgcc -lssl -lcrypto -lstdc++ -lodbc -lodbcinst
CCOPTS    = -DUNICODE -std=c++1z

testcmd : testcmd.o 
	cd ../../../YaizuComLib/src/stkpl; make
	cp ../../../YaizuComLib/src/stkpl/stkpl.a .
	cd ../../../YaizuComLib/src/commonfunc; make
	cp ../../../YaizuComLib/src/commonfunc/commonfunc.a .

	g++ -o testcmd testcmd.o stkpl.a commonfunc.a $(LDFLAGS) $(LIBS)

testcmd.o : testcmd.cpp
	g++ -c testcmd.cpp $(CCOPTS)

clean :
	rm -f *.o *.a testcmd
	cd ../../../YaizuComLib/src/stkpl; make clean
	cd ../../../YaizuComLib/src/commonfunc; make clean

all : clean testcmd
