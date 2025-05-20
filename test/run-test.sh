rm -rf build/ && mkdir build
cmake -S . -B build/
cd build && cmake --build .
./main
