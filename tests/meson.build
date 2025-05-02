test_sources = files('vectors.cc')

test_exe = executable('raytracer_tests',
  test_sources,
  include_directories: include_directories('../src'),
  dependencies: [dep_sdl2]  # Or others your tests depend on
)

test('unit tests', test_exe)
