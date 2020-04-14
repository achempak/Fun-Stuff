add_library(MathFunctions mysqrt.cxx)

# We first state that anybody linking to MathFunctions needs to include the current source directory, 
# while MathFunctions itself doesn’t.
# So this can become an INTERFACE usage requirement.
target_include_directories(MathFunctions
            INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
            )