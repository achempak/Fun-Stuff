add_library(MathFunctions mysqrt.cxx)

# Check whether target platform has log and exp functions
include(CheckSymbolExists)
set(CMAKE_REQUIRED_LIBRARIES "m")
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)

if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()

# We first state that anybody linking to MathFunctions needs to include the current source directory, 
# while MathFunctions itself doesn’t.
# So this can become an INTERFACE usage requirement.
target_include_directories(MathFunctions
            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
            )

install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)