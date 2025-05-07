test_sources = [
  'vectors/vector_overloads.cc',
  'vectors/vector_methods.cc',
  'vectors/vector_dot_and_cross.cc',
  'vectors/vector_advanced.cc'
]

fs = import('fs')

foreach test_source : test_sources
  test_name = fs.replace_suffix(test_source, '')
  test_name = test_name.replace('/', '_')

  test_exe = executable(
    test_name,  # Use the filename without extension as the target name
    test_source,
    include_directories: include_directories('../src'),
    dependencies: [dep_sdl2]  # Or others your tests depend on
  )

  test(test_name, test_exe)
endforeach
