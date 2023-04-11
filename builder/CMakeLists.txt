add_executable(builder builder.cpp)

set_target_properties(builder PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )