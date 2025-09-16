set(MAP_SRC_CHOICE
    ""
)

if(USE_TREAP_ACC)
    message(STATUS "Using Treap Node Implementation")
    list(APPEND MAP_SRC_CHOICE map/map_node_treap.c)
else()
    message(STATUS "Using BST Node Implementation")
    list(APPEND MAP_SRC_CHOICE map/map_node.c)
endif()

set(DataStructure_Src ${DataStructure_Src} 
    map/map.h map/map.c 
    map/map_node.h ${MAP_SRC_CHOICE}
    map/unorder_map.c map/unorder_map.h
    PARENT_SCOPE)
