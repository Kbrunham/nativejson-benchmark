ifeq ($(CONFIG),)
	CONFIG := release64
endif

ifeq ($(VERBOSE),)
	VERBOSE := 1
endif

ifeq ($(OS),)
	OS := $(shell uname -s)
endif


all : bin/nativejson_release_x64_gmake
	cd bin && ./nativejson_release_x64_gmake
	cd result && make -f makefile

bin/nativejson_%_gmake : build/gmake/nativejson.make bin/nativejson_%_gmake.a
	cd build/gmake && make -f nativejson.make config=$(CONFIG) verbose=$(VERBOSE)

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

setup :
	@echo "Setting up environment for OS $(OS)"
	cd build && ./premake.sh
ifeq ($(OS),Darwin)
	# Patch generated makefiles to remove -Wl,-x. Bug in premake
	# tracked here: http://industriousone.com/topic/how-remove-flags-ldflags
	sed -i -e 's/-Wl,-x//g' build/gmake/nativejson.make
	sed -i -e 's/-Wl,-x//g' build/gmake/jsonclibs.make
	# Patch generated makefile to replace -L/usr/lib64 with -L/usr/lib to
	# avoid linker warning
	sed -i -e 's/-L\/usr\/lib64/-L\/usr\/lib/g' build/gmake/nativejson.make
	sed -i -e 's/-L\/usr\/lib64/-L\/usr\/lib/g' build/gmake/jsonclibs.make
endif
	
	
	
build/gmake/nativejson.make : setup
build/gmake/jsonclibs.make : setup

	
	
bin/nativejson_release_x64_gmake.a : build/gmake/jsonclibs.make
	cd build/gmake && make -f jsonclibs.make config=$(CONFIG) verbose=$(VERBOSE)
	
	
	
clean_status :
	@echo "Filesystem status according to GIT"
	@git clean -dfxn
	
	
