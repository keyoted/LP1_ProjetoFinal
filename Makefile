.PHONY: build_debug
.PHONY: build
.PHONY: clear

build_debug:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ./; make

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ./; make

clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles