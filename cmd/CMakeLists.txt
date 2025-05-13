cmake_minimum_required(VERSION 3.10)

project(data4sql)

# For ccls config
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# 将src文件夹下所有文件添加至SOURCE_src
aux_source_directory(./src/ SOURCE_SRC)
aux_source_directory(./../core/src/ SOURCE_CORE)

add_executable(
        data4sql
        main.cpp
        src/easylogging++.cpp
        # cmd
        ${SOURCE_SRC}
        # core
        ${SOURCE_CORE}
)

target_include_directories(
        data4sql
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/.
        ${PROJECT_SOURCE_DIR}/../core/include/.
)

target_compile_features(
        data4sql
        PRIVATE
        cxx_std_17
)