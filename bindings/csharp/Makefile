BUILD_DIR = build

STATIC_DLL = $(BUILD_DIR)/static/Xamarin.Flex.dll
DYNAMIC_DLL = $(BUILD_DIR)/dynamic/Xamarin.Flex.dll

all:    $(STATIC_DLL) $(DYNAMIC_DLL)

base.cs:
	/usr/bin/ruby tools/generate_base.cs

$(STATIC_DLL): base.cs flex.cs
	/bin/mkdir -p `/usr/bin/dirname $(STATIC_DLL)`
	mcs -t:library -define:LIBFLEX_STATIC -out:$(STATIC_DLL) base.cs flex.cs

$(DYNAMIC_DLL): base.cs flex.cs
	/bin/mkdir -p `/usr/bin/dirname $(DYNAMIC_DLL)`
	mcs -t:library -out:$(DYNAMIC_DLL) base.cs flex.cs

clean:
	/bin/rm -rf build
