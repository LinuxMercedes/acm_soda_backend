CPPFLAGS= -O3
all: menuTest checkSodas menuTestWDriver vendSoda vendSodaDaemon

menuTest: utils.o menu.o serialController.o
	g++ menu.o utils.o serialController.o  -o menuTest

menuTestWDriver: utils.o menuWDriver.o serialController.o serialDriver.o
	g++ menuWDriver.o utils.o serialController.o serialDriver.o -o menuTestWDriver

checkSodas: utils.o checkSodas.o serialController.o
	g++ checkSodas.o utils.o serialController.o  -o checkSodas

vendSoda: vendSoda.o serialDriver.o serialController.o utils.o
	g++ vendSoda.o serialDriver.o serialController.o utils.o -o vendSoda

vendSodaDaemon: vendSodaDaemon.o serialDriver.o serialController.o utils.o
	g++ vendSodaDaemon.o serialDriver.o serialController.o utils.o -o vendSodaDaemon

serialController.o: serialController.h serialController.cpp
serialDriver.o: serialController.h serialDriver.h serialDriver.cpp
checkSodas.o: serialController.h checkSodas.cpp
utils.o: utils.h utils.cpp
menu.o: utils.h menu.cpp serialController.h
menuWDriver.o: utils.h menuWDriver.cpp serialDriver.h serialController.h
vendSoda.o: vendSoda.cpp utils.h serialDriver.h serialController.h
vendSodaDaemon.o: vendSodaDaemon.cpp utils.h serialDriver.h serialController.h


clean:
	rm -f *.o menuTest checkSodas menuTestWDriver vendSoda vendSodaDaemon
