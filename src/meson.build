dep_sdl2 = dependency('sdl2', required: true)
dep_sfml = dependency('sfml-graphics', required: true)

all_sources = files(
  'window/screen.cc',
  'window/multi_threaded/screen.cc',
  'geometry/triangle.cc',
  'geometry/sphere.cc',
  'camera/camera.cc',
  'raylogic/raylogic.cc',
  'window/scene_objects/scene_objects.cc',
  'colour/colour.cc'
)

executable('raytracer', 
  sources: ['main.cpp'] + all_sources,
  include_directories: include_directories('window', 'vectors', 'geometry'),
  dependencies: [dep_sdl2] + [dep_sfml]
)
