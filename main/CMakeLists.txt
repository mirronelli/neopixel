# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set (COMPONENT_SRCS
  "main.cpp"
  "neopixel.cpp"
  "effects/snake.cpp"
  "effects/stars.cpp"
  "effects/effect.cpp"
  "effects/police.cpp"
  "effects/rainbow.cpp"
  "effects/effectFactory.cpp"
  "mBle.c"
)

set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()
component_compile_options(-std=c++17)