add_executable(
    cminusfc
    cminusfc.cpp
    cminusf_builder.cpp
)

target_link_libraries(
    cminusfc
    BACK_lib
    OP_lib
    IR_lib
    common
    syntax
)

install(
    TARGETS cminusfc
    RUNTIME DESTINATION bin
)


