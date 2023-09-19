project('Lumos', ['cpp', 'c'],
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=c++11', 'werror=true']
)

compiler = meson.get_compiler('cpp')
openGLDep = dependency('gl')
glewDep = dependency('glew')
glfwDep = dependency('glfw3')
spdlogDep = dependency('spdlog')
freetypeDep = dependency('freetype2')

executable('debug', 'Examples/primitive.cpp', dependencies: [glfwDep, glewDep, openGLDep, spdlogDep, freetypeDep])
