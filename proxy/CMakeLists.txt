add_executable(proxy proxy.cpp)

set_target_properties(proxy PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )