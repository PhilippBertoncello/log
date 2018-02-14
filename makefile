default:	./build/build.out

./build/build.out:	./src/main.cpp ./src/log.cpp
	g++ -o ./build/build.out ./src/main.cpp -L./src/log.cpp
	sudo chmod a+x ./build/build.out

run:	./build/build.out
	./build/build.out

clean:
	rm ./build/build.out
