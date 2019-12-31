.PHONY: build
.PHONY: build_release
.PHONY: clear

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "DEBUG BUILD"

build_release:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "RELEASE BUILD"

clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles

run:
	bin/main.x86