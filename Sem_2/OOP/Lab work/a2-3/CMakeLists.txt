cmake_minimum_required(VERSION 3.26)
project(a2_3_maraielciu1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(a2_3_maraielciu1
        Domain/Domain.c
        Domain/Domain.h
        Domain/DynamicArray.c
        Domain/DynamicArray.h
        UndoRedo/Operation.c
        UndoRedo/Operation.h
        Repo/Repository.c
        Repo/Repository.h
        Service/Service.c
        Service/Service.h
        Tests/Test.c
        Tests/Test.h
        ui.c
        UndoRedo/UndoRedoRepo.c
        UndoRedo/UndoRedoRepo.h)
