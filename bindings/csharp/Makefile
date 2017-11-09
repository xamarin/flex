VERSION = 0.3.2

BUILD_DIR = build

SRCS = base.cs flex.cs

STATIC_DLL = $(BUILD_DIR)/static/Xamarin.Flex.dll
DYNAMIC_DLL = $(BUILD_DIR)/dynamic/Xamarin.Flex.dll
all:    msbuild-update-nuget

base.cs:        ../../flex.h
	/usr/bin/ruby tools/generate_base.rb

$(STATIC_DLL): $(SRCS)
	/bin/mkdir -p `/usr/bin/dirname $(STATIC_DLL)`
	mcs -t:library -define:LIBFLEX_STATIC -out:$(STATIC_DLL) $(SRCS)

$(DYNAMIC_DLL): $(SRCS)
	/bin/mkdir -p `/usr/bin/dirname $(DYNAMIC_DLL)`
	mcs -t:library -out:$(DYNAMIC_DLL) $(SRCS)

clean:
	/bin/rm -rf build

msbuild-update-nuget: 
	nuget restore Xamarin.Flex.sln
	msbuild Xamarin.Flex.sln
	nuget update Xamarin.Flex.sln
