.PHONY: build build_release clean print clip run edit dbg valgrind format

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "DEBUG BUILD"

build_release:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "RELEASE BUILD"

# From utility.sh
clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles

print:
	cat ./test.txt | grep -vE "(^#.*)"

clip:
	(cat ./test.txt | grep -vE "(^#.*)") | xclip -sel clip

run:
	./bin/main.x86

make:
	cd ./build; make

edit:
	${EDITOR} ./test.txt

dgb:
	(((cat ./test.txt | grep -vE "(^#.*)") && cat) | ./bin/main.x86)

valgrind:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --suppressions=./valgrind.supp ./bin/main.x86

format:
	clang-format-7 -i src/*