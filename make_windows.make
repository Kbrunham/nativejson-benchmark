ifeq ($(CONFIG),)
	CONFIG := release64
endif

ifeq ($(VERBOSE),)
	VERBOSE := 1
endif

ifeq ($(OS),)
	OS := $(shell uname -s)
endif


all : bin/nativejson_release_x64_vs2010.exe bin/jsonstat/jsonstat_rapidjson_release_x64_vs2010.exe
	cd bin && ./nativejson_release_x64_vs2010.exe
	cd result && make -f makefile

bin/nativejson_release_x64_vs2010.exe : build/vs2010/benchmark.sln
	cd build/vs2010 && devenv /upgrade benchmark.sln
	msbuild.exe build/vs2010/benchmark.sln  /p:Configuration="release" /p:Platform=x64

bin/jsonstat/jsonstat_rapidjson_release_x64_vs2010.exe : build/vs2010/jsonstat.sln
	cd build/vs2010 && devenv /upgrade jsonstat.sln
	msbuild.exe build/vs2010/jsonstat.sln  /p:Configuration="release" /p:Platform=x64

build/vs2010/benchmark.sln : 
	@echo "Setting up environment for OS $(OS)"
	cd build && ./premake.bat

