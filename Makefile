build_dir=build
exe=act

default: src/**.c src/**.h
	@cp src/**.h include/
	@cd $(build_dir) && meson configure -Ddocs=false && meson compile

run: default
	@./$(build_dir)/$(exe)

test: tests/**.c
	@cd $(build_dir) && meson test

docs: src/**.c src/**.h
	@cd $(build_dir) && meson configure -Ddocs=true && meson compile
