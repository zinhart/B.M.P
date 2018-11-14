# temp build command
mkdir -p build 
cmake -Bbuild -H./ 
make -C build/unit_tests

