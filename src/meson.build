dep_sdl2 = dependency('sdl2', required: true)

window_sources = files(
  'window/Screen.cc'
)

executable('raytracer', 'main.cpp',
  window_sources,
  include_directories: include_directories('window'),
  dependencies: [dep_sdl2]
)
