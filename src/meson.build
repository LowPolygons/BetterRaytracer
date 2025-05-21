dep_sdl2 = dependency('sdl2', required: true)
dep_sfml = dependency('sfml-graphics', required: true)

window_sources = files(
  'window/Screen.cc'
)

vectors_sources = files()

geometry_sources = files(
  'geometry/triangle.cc',
  'geometry/sphere.cc',
  'camera/camera.cc',
  'raylogic/raylogic.cc'
)

executable('raytracer', 
  sources: ['main.cpp'] + window_sources + vectors_sources + geometry_sources,
  include_directories: include_directories('window', 'vectors', 'geometry'),
  dependencies: [dep_sdl2] + [dep_sfml]
)
