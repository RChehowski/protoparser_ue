cmake_minimum_required(VERSION 3.8)
project(protoparser_ue)

set(CMAKE_CXX_STANDARD 11)


include_directories(../Clion-proto)
include_directories(../grpc/third_party/protobuf/src)

set(SOURCE_FILES main.cpp)

set(executable_name Hello_cpp)

link_libraries(-L/Volumes/Samsung/Projects/boh_ue/BloodOfHeroes/Plugins/VizorGrpc/grpc_libs/Mac/Release -lgrpc++ -lprotobuf)

add_executable(protoparser_ue ${SOURCE_FILES})