add_library(project_options INTERFACE)

if(MSVC)
    target_compile_options(project_options INTERFACE
        $<$<CONFIG:Debug>:/W4 /Zi /Od>
        $<$<CONFIG:Release>:/W4 /O2 /DNDEBUG>
    )
else()
    target_compile_options(project_options INTERFACE
        $<$<CONFIG:Debug>:-Wall -Wextra -g -Wimplicit-fallthrough -O0 -Werror=return-type -fsanitize=address>
        $<$<CONFIG:Release>:-Wall -Wextra -O3 -DNDEBUG -Werror=return-type>
    )
    target_link_options(project_options INTERFACE
        $<$<CONFIG:Debug>:-fsanitize=address>
    )
endif()