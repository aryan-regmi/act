source_dirs = src/types/**.c src/interfaces/**.c
header_dirs = src/types/**.h src/interfaces/**.h
build_dir = build
exe = act

default: $(source_dirs) $(header_dirs)
	@cd $(build_dir) && meson configure -Ddocs=false --buildtype debug && meson compile 

release: $(source_dirs) $(header_dirs)
	@cd $(build_dir) && meson configure -Ddocs=false --buildtype release && meson compile 

run: default
	@./$(build_dir)/$(exe)

test: tests/**.c
	@cd $(build_dir) && meson test

docs: src/**.c src/**.h
	@cd $(build_dir) && meson configure -Ddocs=true && meson compile
