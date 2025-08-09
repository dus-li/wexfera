# SPDX-License-Identifier: GPL-3.0-only
# SPDX-FileCopyrightText: Duszku

project('wexfera', 'c', version: '0.0.1')

fs = import('fs')

sources = []
drivers = []
subdir('src')

objcopy = find_program('arm-none-eabi-objcopy')

add_project_link_arguments([], language: 'c')

pwd = meson.current_source_dir()

# Get board value
board = get_option('board')
board_dir = 'board/' + board
if not fs.exists(pwd + '/' + board_dir)
  error('Unknown board: ' + board)
endif

subdir(board_dir)

common_args = [
  '-target', 'armv7m-none-eabi',
  '-mthumb',
  '-Wall',
  '-O0'
]

includes = include_directories([
  'include',
  'board/' + board + '/include'
])

foreach driver : drivers
  subdir('src/drivers/' + driver)
endforeach

# Pull memory linker script for a board
memory_ld = configure_file(
  input:  pwd + '/board/' + board + '/memory.ld',
  output: 'memory.ld',
  copy:   true
)

c_args       = [ common_args, board_args ]
c_link_args  = [ common_args, board_args, '-nostdinc', '-nostdlib' ]
c_link_args += '-T' + pwd + '/src/boot/common.ld'

elf_basename  = 'wexfera'
elf_extension = 'elf'
elf_out       = elf_basename + '.' + elf_extension

c_args += '-DBUILD_VERSION=@0@'.format(meson.project_version())

elf = executable(
  elf_basename,
  sources,
  name_suffix:         elf_extension,
  c_args:              c_args,
  link_args:           c_link_args,
  include_directories: includes
)

img_out = 'Image'
img = custom_target(
  output:           img_out,
  build_by_default: true,
  command:          [objcopy, ['-O', 'binary', '-S', elf.full_path(), img_out]],
  depends:          elf
)
