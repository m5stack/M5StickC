set(COMPONENT_SRCDIRS src 
                      src/Fonts
                      src/utility)

set(COMPONENT_ADD_INCLUDEDIRS src)

set(COMPONENT_PRIV_INCLUDEDIRS src/Fonts 
                               src/Fonts/Custom 
                               src/Fonts/GFXFF 
                               src/Fonts/TrueType 
                               src/utility)

set(COMPONENT_PRIV_REQUIRES arduino)

register_component()
