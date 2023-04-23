add_executable(interpreter interpreter.cpp)

set_target_properties(interpreter PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )