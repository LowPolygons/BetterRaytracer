cmake_minimum_required(VERSION 3.23)

project(Raytracer LANGUAGES CXX)

add_executable(Raytracer)

target_compile_features(Raytracer PRIVATE cxx_std_23)
target_compile_options(Raytracer PRIVATE -stdlib=libc++)
target_link_options(Raytracer PRIVATE -stdlib=libc++)

target_sources(Raytracer
  PRIVATE
  src/image/multi_threaded/image.cc
  src/camera/camera.cc
  src/raylogic/raylogic.cc
  src/scene/scene_objects/scene_objects.cc
  src/image/image.cc
  src/colour/colour.cc
  src/scene/SceneConfig.cc
  src/readers/file_reader.cc
  src/readers/scene_config/ConfigReader.cc
  src/readers/object_config/ColourReader.cc
  src/readers/object_config/ObjectConfig.cc
  src/raytracer.cc
  src/bmp_writer/bmp.cc
  src/rasteriser.cc
  src/image/sycl_implementation/sycl_impl.cc
  src/main.cpp
)

target_include_directories(Raytracer
  PRIVATE
  src
)
