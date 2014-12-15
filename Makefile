ifeq ($(CONFIG),)
	CONFIG := release64
endif

ifeq ($(VERBOSE),)
	VERBOSE := 1
endif

ifeq ($(OS),)
	OS := $(shell uname -s)
endif


all :
ifeq ($(OS),Windows_NT)
	$(MAKE) -f make_windows.make
else
	$(MAKE) -f make_linux.make
endif

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
	
	
