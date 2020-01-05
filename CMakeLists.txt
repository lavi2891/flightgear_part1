cmake_minimum_required(VERSION 3.15)
project(project_part_1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(project_part_1 main.cpp Expression.h Expression.cpp Exp_Interpreter.h Exp_Interpreter.cpp Command.h main.h Command.cpp Condition.h Condition.cpp Variable.h Global.h)