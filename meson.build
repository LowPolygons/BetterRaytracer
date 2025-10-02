project('raytracer', 'cpp',
  default_options: ['cpp_std=c++23'] + ['cpp_args=-fms-extensions']
)

inc_dirs = include_directories('src')

all_sources = files(
  './src/image/multi_threaded/image.cc',
  # './src/geometry/triangle.cc',
  # './src/geometry/sphere.cc',
  './src/camera/camera.cc',
  './src/raylogic/raylogic.cc',
  './src/scene/scene_objects/scene_objects.cc',
  './src/image/image.cc',
  './src/colour/colour.cc',
  './src/scene/SceneConfig.cc',
  './src/readers/file_reader.cc',
  './src/readers/scene_config/ConfigReader.cc',
  './src/readers/object_config/ColourReader.cc',
  './src/readers/object_config/ObjectConfig.cc',
  './src/raytracer.cc',
  './src/bmp_writer/bmp.cc',
  './src/rasteriser.cc',
  './src/image/sycl_implementation/sycl_impl.cc'
)

sdl_dependency = dependency('sdl2', required: false)
sycl_dependency = dependency('hipSYCL', modules: ['hipSYCL::acpp-common'], required: false)

if sycl_dependency.found()
  add_project_arguments('-DSYCL_DEPENDENCY_FOUND', language: 'cpp')
else
  message('SYCL not found: Defaulting to software')
endif 

if sdl_dependency.found()
  add_project_arguments('-DSDL2_DEPENDENCY_FOUND', language: 'cpp')
else 
  message('WARNING: SDL2 Not found. Install SDl2 if possible, meanwhile program will run without a Preview Option')
endif 

executable('raytracer', 
  sources: ['./src/main.cpp'] + all_sources,
  include_directories: [
    include_directories('./src/image', './src/vectors', './src/geometry'),
    inc_dirs
  ],
  dependencies: [sdl_dependency] + [sycl_dependency]
)

subdir('tests')

