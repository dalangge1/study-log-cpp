echo '============class============'
clang++ -cc1 -fdump-record-layouts -emit-llvm class.cpp
echo '============struct============'
clang++ -cc1 -fdump-record-layouts -emit-llvm struct.cpp