config_data.set('INPUT', meson.source_root())
config_data.set('OUTPUT', meson.source_root() + '/doc')

doxyfile = configure_file(input: 'Doxyfile.in',
                          output: 'Doxyfile',
                          configuration: config_data,
                          install: false)

datadir = join_paths(get_option('datadir'), 'doc', 'act')

html_target = custom_target('spede-docs',
                            input: doxyfile,
                            output: 'html',
                            command: [doxygen, doxyfile,  '@INPUT@'],
                            install: true,
                            install_dir: datadir)
