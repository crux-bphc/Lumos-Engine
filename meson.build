project('Lumos', ['cpp', 'c'],
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++20']
)

compiler = meson.get_compiler('cpp')
openGLDep = dependency('gl')
glewDep = dependency('glew')
glfwDep = dependency('glfw3')
spdlogDep = dependency('spdlog')
openalDep = dependency('openal')
alut_dep = dependency('freealut')

incdir = include_directories('include')
extdir = include_directories('external')
stb_src = ['external/stb/stb.cpp']

build_src =  ['src/lumos.cpp',
              'src/gl_stuff.cpp',
              'src/shader.cpp',
              'src/shapes.cpp',
              'src/textures.cpp',
              'src/batch_renderer.cpp',
              'src/audio.cpp']

to_build = ['examples/openal_test.cpp']

build_src += stb_src
build_src += to_build

executable(
  'debug', build_src, 
  dependencies: [glfwDep, glewDep, openGLDep, spdlogDep, openalDep, alut_dep], 
  include_directories : [incdir, extdir],
  # Enable for profiling
  # cpp_args: ['-pg', '-no-pie'],
  # link_args: ['-pg', '-no-pie'],
  )
