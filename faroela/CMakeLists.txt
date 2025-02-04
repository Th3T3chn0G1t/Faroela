add_library(faroela STATIC)

if(${FARO_ENABLE_ASAN})
    target_cxx_multi_compile_link_options_conditional(FARO PRIVATE -fsanitize=address)
    cxx_options_conditional(
            COMMAND target_compile_options
            ARGS FARO PRIVATE
            FLAGS /fsanitize=address)
endif()

if(${FARO_ENABLE_UBSAN})
    target_cxx_multi_compile_link_options_conditional(FARO PRIVATE -fsanitize=undefined)
endif()

if(${FARO_ENABLE_MSAN})
    target_cxx_multi_compile_link_options_conditional(FARO PRIVATE -fsanitize=memory)
endif()

if(${FARO_ENABLE_TSAN})
    target_cxx_multi_compile_link_options_conditional(FARO PRIVATE -fsanitize=thread)
endif()

if(${FARO_ENABLE_CFISAN})
    if(${IPO_SUPPORTED})
        target_cxx_multi_compile_link_options_conditional(FARO PRIVATE -fsanitize=cfi -fvisibility=default -flto)
    endif()
endif()

cxx_options_conditional(
        COMMAND target_compile_options
        ARGS FARO PRIVATE
        FLAGS -Wall -Wextra -Werror -pedantic -pedantic-errors)

cxx_options_conditional(
        COMMAND target_compile_options
        ARGS FARO PRIVATE
        FLAGS /Wall /WX
            /wd4820 # Padding
            /wd4710 # Function was not inlined
            /wd5045 # Spectre mitigations
            /wd4711 # Function selected for automatic inlining
            /wd4706 # Assignment inside conditional expression (even with double paren)
            /wd4061 # Non-explicitly handled enum value (Doesn't count `default:')
            /wd4127) # Conditional expression is constant (Even if block contains a `break')
