BUILD_DIR = build

SRCS = base.cs flex.cs

STATIC_DLL = $(BUILD_DIR)/static/Xamarin.Flex.dll
DYNAMIC_DLL = $(BUILD_DIR)/dynamic/Xamarin.Flex.dll

all:    nuget

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

nuget:  $(STATIC_DLL) $(DYNAMIC_DLL)
	(cd ../.. && nuget pack bindings/csharp/nuget/Xamarin.Flex.nuspec -BasePath . -OutputDirectory bindings/csharp)
