cmake_minimum_required(VERSION 3.10)

project(data4sql)

# For ccls config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src/ SOURCE_SRC)

add_executable(
        data4sql
        main.cpp
        # core
        ${SOURCE_SRC}
        src/table.cpp
        include/table.h
        src/DataTypes.cpp
        include/DataTypes.h
)

target_include_directories(
        data4sql
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/.
)

target_compile_features(
        data4sql
        PRIVATE
        cxx_std_17
)