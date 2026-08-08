# CMake generated Testfile for 
# Source directory: /home/ubuntu/house-flipper-user
# Build directory: /home/ubuntu/house-flipper-user/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[house_flipper_tests]=] "/home/ubuntu/house-flipper-user/build/house_flipper_tests")
set_tests_properties([=[house_flipper_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/ubuntu/house-flipper-user/CMakeLists.txt;40;add_test;/home/ubuntu/house-flipper-user/CMakeLists.txt;0;")
subdirs("_deps/raylib-build")
