ifeq ($(CONFIG),)
	CONFIG := release64
endif

ifeq ($(VERBOSE),)
	VERBOSE := 1
endif

ifeq ($(OS),)
	OS := $(shell uname -s)
endif


all : data/citm_catalog.json
ifeq ($(OS),Windows_NT)
	$(MAKE) -f make_windows.make
else
	$(MAKE) -f make_linux.make
endif

data/citm_catalog.json : thirdparty/json-parsers-benchmark/data/citm_catalog.json
	cp -f $< $@	

data/large-dict.json : thirdparty/json-benchmarks/data/large-dict.json
	cp -f $< $@	

data/medium-dict.json : thirdparty/json-benchmarks/data/medium-dict.json
	cp -f $< $@	

data/citylots.json : thirdparty/sf-city-lots-json/citylots.json
	cp -f $< $@	

clean : 
	rm -rf build/gmake
	rm -rf build/vs2005
	rm -rf build/vs2008
	rm -rf build/vs2010
	rm -rf intermediate
	rm -rf src/machine.h
	rm -rf bin
	rm -rf result/*.csv
	rm -rf result/*.html
	
clean_status :
	@echo "Filesystem status according to GIT"
	@git clean -dfxn
	
	
