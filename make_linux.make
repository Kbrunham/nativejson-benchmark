ifeq ($(CONFIG),)
	CONFIG := release64
endif

ifeq ($(VERBOSE),)
	VERBOSE := 1
endif

ifeq ($(OS),)
	OS := $(shell uname -s)
endif

ifeq ($(NOMACHINE),)
	MACHINE_H_FILE = src/machine.h
endif


PATCHED_MAKE_FILE_TARGETS = \
	intermediate/benchmark.make.orig \
	intermediate/jsonclibs.make.orig \
	intermediate/jsonclibs2.make.orig \
	intermediate/jsonstat.make.orig \
	intermediate/jsonstat_cajuntest.make.orig \
	intermediate/jsonstat_casablancatest.make.orig \
	intermediate/jsonstat_cjsontest.make.orig \
	intermediate/jsonstat_dropboxjson11test.make.orig \
	intermediate/jsonstat_fastjsontest.make.orig \
	intermediate/jsonstat_gasontest.make.orig \
	intermediate/jsonstat_janssontest.make.orig \
	intermediate/jsonstat_jsonboxtest.make.orig \
	intermediate/jsonstat_jsoncpptest.make.orig \
	intermediate/jsonstat_jsonctest.make.orig \
	intermediate/jsonstat_jsonspirittest.make.orig \
	intermediate/jsonstat_jsonxxtest.make.orig \
	intermediate/jsonstat_parsontest.make.orig \
	intermediate/jsonstat_picojsontest.make.orig \
	intermediate/jsonstat_rapidjsontest.make.orig \
	intermediate/jsonstat_simplejsontest.make.orig \
	intermediate/jsonstat_strduptest.make.orig \
	intermediate/jsonstat_udpjsontest.make.orig \
	intermediate/jsonstat_ujson4c.make.orig \
	intermediate/jsonstat_vincenthzlibjsontest.make.orig \
	intermediate/jsonstat_yajltest.make.orig \
	intermediate/nativejson.make.orig

.PRECIOUS: build/gmake/%.make

all : code
	cd bin && ./nativejson_release_x64_gmake
	cd result && make -f makefile

code : bin/nativejson_release_x64_gmake bin/jsonstat/jsonstat_rapidjson_release_x64_gmake

build/gmake/%.make : $(MACHINE_H_FILE)
	@echo "Setting up environment for OS $(OS)"
	cd build && ./premake.sh

src/machine.h :
	cd build && ./machine.sh

bin/nativejson_release_x64_gmake.a : build/gmake/jsonclibs.make $(PATCHED_MAKE_FILE_TARGETS)
	cd build/gmake && make -f jsonclibs.make config=$(CONFIG) verbose=$(VERBOSE)
	
bin/jsonstat/jsonstat_rapidjson_release_x64_gmake : build/gmake/jsonstat.make $(PATCHED_MAKE_FILE_TARGETS)
	cd build/gmake && make -f jsonstat.make config=$(CONFIG) verbose=$(VERBOSE)

bin/nativejson_release_x64_gmake : build/gmake/nativejson.make bin/nativejson_release_x64_gmake.a $(PATCHED_MAKE_FILE_TARGETS)
	cd build/gmake && make -f nativejson.make config=$(CONFIG) verbose=$(VERBOSE)

intermediate :
	mkdir $@

intermediate/%.make.orig : build/gmake/%.make intermediate
ifeq ($(OS),Darwin)
	# Patch generated makefiles to remove -Wl,-x. Bug in premake
	# tracked here: http://industriousone.com/topic/how-remove-flags-ldflags
	sed -i -e 's/-Wl,-x//g' $<
	# Patch generated makefile to replace -L/usr/lib64 with -L/usr/lib to
	# avoid linker warning
	sed -i -e 's/-L\/usr\/lib64/-L\/usr\/lib/g' $<
endif
	cp  -f $< $@

clean_status :
	@echo "Filesystem status according to GIT"
	@git clean -dfxn
	
	
