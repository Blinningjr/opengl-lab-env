#--------------------------------------------------------------------------
# triangulation3d project
#--------------------------------------------------------------------------

FILE(GLOB triangulation3d_headers code/*.h)
FILE(GLOB triangulation3d_sources code/*.cc)

SET(files_triangulation3d ${triangulation3d_headers} ${triangulation3d_sources})
SOURCE_GROUP("triangulation3d" FILES ${files_triangulation3d})

ADD_EXECUTABLE(triangulation3d ${files_triangulation3d})
TARGET_LINK_LIBRARIES(triangulation3d core render)
ADD_DEPENDENCIES(triangulation3d core render)