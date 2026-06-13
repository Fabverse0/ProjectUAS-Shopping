cmake_minimum_required(VERSION 3.16)

project(C-Cart VERSION 1.0 LANGUAGES CXX)

# =========================
# KONFIGURASI PROYEK
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# KONFIGURASI QT
# =========================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# =========================
# SOURCE GROUPS
# =========================
set(MODEL_SOURCES
    src/models/product.cpp
    src/models/order.cpp
)

set(STRUCTURE_SOURCES
    src/structures/avltree.cpp
    src/structures/bst.cpp
    src/structures/cart_list.cpp
    src/structures/catalog_list.cpp
    src/structures/promo_list.cpp
    src/structures/navigation_stack.cpp
    src/structures/order_queue.cpp
    src/structures/graph.cpp
)

set(SERVICE_SOURCES
    src/services/product_service.cpp
    src/services/cart_service.cpp
    src/services/checkout_service.cpp
    src/services/route_service.cpp
)

set(UI_SOURCES
    src/ui/mainwindow.cpp
    src/ui/mainwindow.ui
)

set(PROJECT_INCLUDES
    src/models
    src/structures
    src/services
    src/ui
)

# =========================
# MAIN APPLICATION
# =========================
add_executable(C-Cart
    main.cpp
    ${MODEL_SOURCES}
    ${STRUCTURE_SOURCES}
    ${SERVICE_SOURCES}
    ${UI_SOURCES}
)

target_include_directories(C-Cart PRIVATE ${PROJECT_INCLUDES})
target_link_libraries(C-Cart PRIVATE Qt6::Widgets)

# =========================
# TEST PRODUCT MODULE
# =========================
add_executable(test_product
    test_product_module.cpp
    src/models/product.cpp
    src/structures/avltree.cpp
    src/structures/bst.cpp
    src/services/product_service.cpp
)

target_include_directories(test_product PRIVATE ${PROJECT_INCLUDES})

# =========================
# TEST CART MODULE
# =========================
add_executable(test_cart
    test_cart.cpp
    src/models/product.cpp
    src/structures/cart_list.cpp
    src/structures/catalog_list.cpp
    src/structures/promo_list.cpp
    src/structures/navigation_stack.cpp
    src/services/cart_service.cpp
)

target_include_directories(test_cart PRIVATE ${PROJECT_INCLUDES})

# =========================
# TEST CHECKOUT MODULE
# =========================
add_executable(test_checkout
    test_checkout_module.cpp
    src/models/order.cpp
    src/structures/order_queue.cpp
    src/structures/graph.cpp
    src/services/checkout_service.cpp
    src/services/route_service.cpp
)

target_include_directories(test_checkout PRIVATE ${PROJECT_INCLUDES})