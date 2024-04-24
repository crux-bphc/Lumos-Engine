project('Lumos', ['cpp', 'c'],
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++20']
)

compiler = meson.get_compiler('cpp')
openGLDep = dependency('gl')
glewDep = dependency('glew')
glfwDep = dependency('glfw3')
spdlogDep = dependency('spdlog')
incdir = include_directories('include/')
extdir = include_directories('external/')
enttdir = include_directories('external/entt/src/')
glmdir = include_directories('external/glm/')
stb_src = ['external/stb/stb.cpp']

build_src =  ['src/lumos.cpp',
              'src/gl_stuff.cpp',
              'src/shader.cpp',
              'src/shapes.cpp',
              'src/textures.cpp',
              'src/input.cpp']


to_build = ['examples/checkerboard.cpp']

build_src += stb_src
build_src += to_build

executable(
  'debug', build_src, 
  dependencies: [glfwDep, glewDep, openGLDep, spdlogDep], 
  include_directories : [incdir, extdir, enttdir, glmdir],
  ## Enable for examples that use shapes.cpp
  cpp_args: ['-DLEGACY'],
  # Enable for profiling
  # cpp_args: ['-pg', '-no-pie', '-fno-builtin'],
  # link_args: ['-pg', '-no-pie', '-fno-builtin'],
  )
