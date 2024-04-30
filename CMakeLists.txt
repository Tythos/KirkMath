cmake_minimum_required(VERSION 3.14)
project(KirkMath)
add_library(${PROJECT_NAME} STATIC
    "Mat.cpp"
    "Quat.cpp"
    "Trans.cpp"
    "Vec.cpp"
    "numerics.cpp"
)
