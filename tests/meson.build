global_implementations = [
  '../src/geometry/triangle.cc',
  '../src/geometry/sphere.cc'
]

included_directories = [
  '../src',
  '../src/vectors',
  '../src/geometry',
  '../src/colour'
]

test_sources = [
  'vectors/vector_overloads.cc',
  'vectors/vector_methods.cc',
  'vectors/vector_dot_and_cross.cc',
  'vectors/vector_advanced.cc',
  'geometry/triangle.cc',
  'geometry/sphere.cc'
]

fs = import('fs')

foreach test_source : test_sources
  test_name = fs.replace_suffix(test_source, '')
  test_name = test_name.replace('/', '_')

  test_exe = executable(
    test_name,  # Use the filename without extension as the target name
    sources: [test_source] + global_implementations,
    include_directories: include_directories(included_directories),
  )

  test(test_name, test_exe)
endforeach
