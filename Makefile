.PHONY: usage cmake ninja build debug flash presets clean wipe-tmp
# commands
usage:
	@cat Makefile.usage.txt

cmake:
	@echo "configure project default preset into build"
	@cmake --preset default

ninja:
	@echo "generates project default preset into build"
	@cmake --build --preset default

build:
	@echo "configure and generate project into build"
	@cmake --preset default
	@cmake --build --preset default

debug:
	@echo "configure and generate debug into temp"
	@cmake --preset debug
	@cmake --build --preset debug

flash:
	@echo "configure and generate release into temp"
	@cmake --preset flash
	@cmake --build --preset flash

presets:
	@echo "copy user presets example if not exists"
	@cp -n CMakeUserPresets.example.json CMakeUserPresets.json

clean:
	@echo "remove force recursive build directory"
	@rm -fr build/
	@git restore build/.gitignore

wipe-tmp:
	@echo "remove force recursive temp presets_ folder"
	@rm -fr temp/presets_/
