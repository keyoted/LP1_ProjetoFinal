#! /bin/sh
if [ -z ${2+x} ]; then
    EXENAME='main.x86'
else
    EXENAME=${2}
fi
ROOT_DIR=$(dirname $(realpath $0))
if [ $1 = "print" ]; then
    cat ${ROOT_DIR}/src/test.txt | grep -vE "(^#.*)"
elif [ $1 = "clip" ]; then
    (cat ${ROOT_DIR}/test.txt | grep -vE "(^#.*)") | xclip -sel clip
elif [ $1 = "run" ]; then
    ${ROOT_DIR}/bin/${EXENAME}
elif [ $1 = "make" ]; then
    cd ${ROOT_DIR}/build; make
elif [ $1 = "edit" ]; then
    ${EDITOR} ${ROOT_DIR}/src/test.txt
elif [ $1 = "dbg" ]; then
    (((cat ${ROOT_DIR}/src/test.txt | grep -vE "(^#.*)") && cat) | ${ROOT_DIR}/bin/${EXENAME})
elif [ $1 = "valgrind" ]; then
    valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --suppressions=${ROOT_DIR}/valgrind.supp ${ROOT_DIR}/bin/${EXENAME}
    #valgrind -s --gen-suppressions=all --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --suppressions=${ROOT_DIR}/valgrind.supp ${ROOT_DIR}/bin/${EXENAME}
else
    echo "utility.sh [action] [EXE_NAME (optional)]"
    echo "Actions:"
    echo "run      - Run exe"
    echo "dbg      - Run with debug actions"
    echo "print    - Print debug actions"
    echo "edit     - Edit debug actions \$EDITOR must be set"
    echo "clip     - Copy debug actions to clipboard"
    echo "make     - Run make"
    echo "valgrind - Run in valgrind"
fi