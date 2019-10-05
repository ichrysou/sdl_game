message("-- Linking libpng16-16")
add_library(libpng16-16 libpng16-16.dll)
set_target_properties(libpng16-16 PROPERTIES LINKER_LANGUAGE C)