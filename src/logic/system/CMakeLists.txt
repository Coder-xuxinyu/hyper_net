
INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/src/API"
    "${PROJECT_SOURCE_DIR}/src/public"
    "${PROJECT_SOURCE_DIR}/src/public/tinyxml"
    "${PROJECT_SOURCE_DIR}/src/logic/interface"
    )

SET(CLUSTER
    "${PROJECT_SOURCE_DIR}/src/logic/cluster/Cluster.h"
    "${PROJECT_SOURCE_DIR}/src/logic/cluster/Cluster.cpp"
    "${PROJECT_SOURCE_DIR}/src/logic/cluster/main.cpp"
    )

SOURCE_GROUP(interface FILES ${LOGIC_INTERFACE})
SOURCE_GROUP(API FILES ${API})
SOURCE_GROUP(public\\tinyxml FILES ${PUBLIC_XML})
SOURCE_GROUP(public FILES ${PUBLIC})
SOURCE_GROUP(\\ FILES ${CLUSTER})

ADD_LIBRARY(Cluster SHARED
        ${CLUSTER}
        ${API}
        ${LOGIC_INTERFACE}
        ${PUBLIC}
        ${PUBLIC_XML}
        )
SET_TARGET_PROPERTIES(Cluster PROPERTIES FOLDER "core")
