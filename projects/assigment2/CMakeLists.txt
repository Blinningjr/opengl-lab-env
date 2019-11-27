#--------------------------------------------------------------------------
# triangulation2d project
#--------------------------------------------------------------------------

FILE(GLOB triangulation2d_headers code/*.h)
FILE(GLOB triangulation2d_sources code/*.cc)

SET(files_triangulation2d ${triangulation2d_headers} ${triangulation2d_sources})
SOURCE_GROUP("triangulation2d" FILES ${files_triangulation2d})

ADD_EXECUTABLE(triangulation2d ${files_triangulation2d})
TARGET_LINK_LIBRARIES(triangulation2d core render)
ADD_DEPENDENCIES(triangulation2d core render)