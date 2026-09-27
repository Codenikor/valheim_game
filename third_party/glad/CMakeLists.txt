# Сгенерировано: glad --api gl:core=4.1 --extensions="" --out-path third_party/glad c
add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC include)