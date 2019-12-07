#--------------------------------------------------------------------------
# simple3DGraphics project
#--------------------------------------------------------------------------

FILE(GLOB simple3DGraphics_headers code/*.h)
FILE(GLOB simple3DGraphics_sources code/*.cc)

SET(files_simple3DGraphics ${simple3DGraphics_headers} ${simple3DGraphics_sources})
SOURCE_GROUP("simple3DGraphics" FILES ${files_simple3DGraphics})

ADD_EXECUTABLE(simple3DGraphics ${files_simple3DGraphics})
TARGET_LINK_LIBRARIES(simple3DGraphics core render)
ADD_DEPENDENCIES(simple3DGraphics core render)