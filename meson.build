project('raytracer', 'cpp',
  default_options: ['cpp_std=c++23'] + ['cpp_args=-fms-extensions']
)

dep_sdl2 = dependency('sdl2', required: true)
dep_sfml = dependency('sfml-graphics', required: true)
dep_opengl = dependency('opengl', required: true)

inc_dirs = include_directories('src', 'external')

all_sources = files(
  './src/window/screen.cc',
  './src/window/multi_threaded/screen.cc',
  './src/geometry/triangle.cc',
  './src/geometry/sphere.cc',
  './src/camera/camera.cc',
  './src/raylogic/raylogic.cc',
  './src/window/scene_objects/scene_objects.cc',
  './src/window/screen_types/screen_nogpu.cc',
  './src/window/screen_types/screen_sfml.cc',
  './src/colour/colour.cc',
  './src/SceneConfig.cc',
  './external/stb/stb_image_write.cc'
)

executable('raytracer', 
  sources: ['./src/main.cpp'] + all_sources,
  include_directories: [
    include_directories('./src/window', './src/vectors', './src/geometry'),
    inc_dirs
  ],
  dependencies: [dep_sdl2] + [dep_sfml] + [dep_opengl]
)

subdir('tests')

