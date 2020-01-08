#--------------------------------------------------------------------------
# graphics3d project
#--------------------------------------------------------------------------

FILE(GLOB graphics3d_headers code/*.h)
FILE(GLOB graphics3d_sources code/*.cc)

SET(files_graphics3d ${graphics3d_headers} ${graphics3d_sources})
SOURCE_GROUP("graphics3d" FILES ${files_graphics3d})

ADD_EXECUTABLE(graphics3d ${files_graphics3d})
TARGET_LINK_LIBRARIES(graphics3d core render)
ADD_DEPENDENCIES(graphics3d core render)