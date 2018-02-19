project('integrityd', 'cpp')

src = ['crc32.cc', 'descriptor.cc', 'main.cc', 'operations.cc']
inc = include_directories('.')

executable('integrityd', src,
           include_directories : inc,
           cpp_args : '-std=c++17',
           link_args : ['-lstdc++fs'])

checkfile_src = ['crc32.cc', 'checkfile.cc']

executable('checkfile', checkfile_src,
           include_directories : inc,
           cpp_args : ['-std=c++17', '-O2'],
           link_args : ['-lstdc++fs'])
