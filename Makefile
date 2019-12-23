.PHONY: build_debug
.PHONY: build
.PHONY: clear

build_debug:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ./; make; echo "DEBUG BUILD"

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ./; make; echo "RELEASE BUILD"

clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles