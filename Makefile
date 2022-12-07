source_dirs = src/core/**.c src/interfaces/**.c
header_dirs = src/core/**.h src/interfaces/**.h
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

docs: $(source_dirs) $(header_dirs)
	@cd $(build_dir) && meson configure -Ddocs=true && meson compile
