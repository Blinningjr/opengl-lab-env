#--------------------------------------------------------------------------
# graphics3d project
#--------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.13)

FILE(GLOB graphics3d_headers code/*.h code/engine/*.h code/engine/io/*.h code/engine/quadTree/*.h
    code/graphicsNodes/*.h code/materials/*.h code/utils/*.h)
FILE(GLOB graphics3d_sources code/*.cc code/engine/*.cc code/engine/io/*.cc code/engine/quadTree/*.cc
    code/graphicsNodes/*.cc code/materials/*.cc code/utils/*.cc)


SET(files_graphics3d ${graphics3d_headers} ${graphics3d_sources})
SOURCE_GROUP("graphics3d" FILES ${files_graphics3d})

ADD_EXECUTABLE(graphics3d ${files_graphics3d})
TARGET_LINK_LIBRARIES(graphics3d core render)
ADD_DEPENDENCIES(graphics3d core render)