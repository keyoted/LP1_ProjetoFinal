.PHONY: build
.PHONY: build_release
.PHONY: clear

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ./; make; echo "DEBUG BUILD"

build_release:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ./; make; echo "RELEASE BUILD"

clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles

run:
	bin/main.x86