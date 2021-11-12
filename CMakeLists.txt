include(dciIntegrationUnit)
dciIntegrationUnit(logger
        INCLUDE_DIRS include)

file(GLOB_RECURSE INC include/*)
file(GLOB_RECURSE SRC src/*)

add_library(${UNAME} STATIC ${INC} ${SRC})
dciIntegrationSetupTarget(${UNAME})
