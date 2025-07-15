project('hashtable', 'c')

src = ['./src/main.c',]

incdir = include_directories('./src')

cc = meson.get_compiler('c')
math = cc.find_library('m')

executable('ht', src, include_directories: incdir, dependencies: [math])
