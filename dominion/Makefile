CFLAGS = -Wall -fpic -coverage -lm

all: playdom player

unittestresults.out: test-build
	# Execute unittest1 and print coverage of tested function
	./unittest1 > unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "whoseTurn" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute unittest2 and print coverage of tested function
	./unittest2 >> unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "handCard" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute unittest3 and print coverage of tested function
	./unittest3 >> unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "fullDeckCount" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute unittest4 and print coverage of tested function
	./unittest4 >> unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "updateCoins" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute cardtest1 and print coverage of tested function
	./cardtest1 >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute cardtest2 and print coverage of tested function
	./cardtest2 >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute cardtest3 and print coverage of tested function
	./cardtest3 >> unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "cardEffectSmithy" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	# Execute cardtest4 and print coverage of tested function
	./cardtest4 >> unittestresults.out
	echo "" >> unittestresults.out
	echo "TEST COVERAGE" >> unittestresults.out
	gcov -f dominion.c | grep -A1 "cardEffectCouncilRoom" >> unittestresults.out

	echo "" >> unittestresults.out
	echo "-------------------------------------------" >> unittestresults.out
	echo "" >> unittestresults.out

	echo "OVERALL COVERAGE of dominion.c" >> unittestresults.out
	gcov dominion.c | grep Lines >> unittestresults.out
	echo "" >> unittestresults.out

test-build: dominion.o test-lib.o
	gcc -o unittest1 unittest1.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o unittest2 unittest2.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o unittest3 unittest3.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o unittest4 unittest4.c -g test-lib.o dominion.o rngs.o $(CFLAGS)

	gcc -o cardtest1 cardtest1.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest2 cardtest2.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest3 cardtest3.c -g test-lib.o dominion.o rngs.o $(CFLAGS)
	gcc -o cardtest4 cardtest4.c -g test-lib.o dominion.o rngs.o $(CFLAGS)

test-lib.o:
	gcc -c test-lib.c -g $(CFLAGS)

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

clean:
	rm -f *.o playdom.exe playdom test.exe test player unittestresults.out unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so *.a
	rm -rf *.dSYM
