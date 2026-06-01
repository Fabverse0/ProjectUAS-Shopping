cmake_minimum_required(VERSION 3.16)

project(C-Cart VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Collect all source files
set(SOURCES
    main.cpp
    src/models/product.cpp
    src/models/order.cpp
    src/structures/avltree.cpp
    src/structures/bst.cpp
    src/structures/cart_list.cpp
    src/structures/catalog_list.cpp
    src/structures/promo_list.cpp
    src/structures/navigation_stack.cpp
    src/structures/order_queue.cpp
    src/structures/graph.cpp
    src/services/product_service.cpp
    src/services/cart_service.cpp
    src/services/checkout_service.cpp
    src/services/route_service.cpp
    src/ui/mainwindow.cpp
)

set(HEADERS
    src/models/product.h
    src/models/order.h
    src/structures/avltree.h
    src/structures/bst.h
    src/structures/cart_list.h
    src/structures/catalog_list.h
    src/structures/promo_list.h
    src/structures/navigation_stack.h
    src/structures/order_queue.h
    src/structures/graph.h
    src/services/product_service.h
    src/services/cart_service.h
    src/services/checkout_service.h
    src/services/route_service.h
    src/ui/mainwindow.h
)

set(UIS
    src/ui/mainwindow.ui
)

add_executable(C-Cart
    ${SOURCES}
    ${HEADERS}
    ${UIS}
)

target_link_libraries(C-Cart PRIVATE Qt6::Widgets)

# Include directories
target_include_directories(C-Cart PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/structures
    ${CMAKE_CURRENT_SOURCE_DIR}/src/services
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
)
