FLEX_SRC = flex.c
FLEX_FILES = flex.c flex.h
BUILD_DIR = ./build

CFLAGS = -Wall -Werror -O3
LDFLAGS = 

MACOS_MIN_TARGET = '10.10'
IOS_MIN_TARGET = '7.0'
ANDROID_API = '15'

XCODE_DIR = /Applications/Xcode.app
XCODE_BIN_DIR = $(XCODE_DIR)/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
XCODE_CC = $(XCODE_BIN_DIR)/clang

ifndef ANDROID_NDK
    $(error The ANDROID_NDK environment variable is not defined)
endif

ANDROID_CC = $(ANDROID_NDK)/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang
ANDROID_BIN_DIR = $(ANDROID_NDK)/toolchains/x86-4.9/prebuilt/darwin-x86_64/i686-linux-android/bin

MACOS_BUILD_DIR = $(BUILD_DIR)/macos
MACOS_FLEX_O = $(MACOS_BUILD_DIR)/flex.o
MACOS_DYLIB = $(MACOS_BUILD_DIR)/libflex.dylib
MACOS_SLIB = $(MACOS_BUILD_DIR)/libflex.a
MACOS_ARCHFLAG = -arch i386 -arch x86_64
MACOS_CFLAGS = -mmacosx-version-min=$(MACOS_MIN_TARGET)

IOS_BUILD_DIR = $(BUILD_DIR)/ios
IOS_DYLIB = $(IOS_BUILD_DIR)/libflex.dylib
IOS_SLIB = $(IOS_BUILD_DIR)/libflex.a

IOS_SIM_PNAME = iPhoneSimulator
IOS_SIM_FLEX_O = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/flex.o
IOS_SIM_DYLIB = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/libflex.dylib
IOS_SIM_SLIB = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/libflex.a
IOS_SIM_ARCHFLAG = -arch i386 -arch x86_64
IOS_SIM_CFLAGS = -isysroot $(XCODE_DIR)/Contents/Developer/Platforms/$(IOS_SIM_PNAME).platform/Developer/SDKs/$(IOS_SIM_PNAME).sdk -mios-simulator-version-min=$(IOS_MIN_TARGET)

IOS_DEV_PNAME = iPhoneOS
IOS_DEV_FLEX_O = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/flex.o
IOS_DEV_DYLIB = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/libflex.dylib
IOS_DEV_SLIB = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/libflex.a
IOS_DEV_ARCHFLAG = -arch armv7 -arch armv7s -arch arm64 
IOS_DEV_CFLAGS = -isysroot $(XCODE_DIR)/Contents/Developer/Platforms/$(IOS_DEV_PNAME).platform/Developer/SDKs/$(IOS_DEV_PNAME).sdk -mios-version-min=$(IOS_MIN_TARGET)

ANDROID_BUILD_DIR = $(BUILD_DIR)/android

ANDROID_X86_DIR = $(ANDROID_BUILD_DIR)/x86
ANDROID_X86_FLEX_O = $(ANDROID_X86_DIR)/flex.o
ANDROID_X86_DYLIB = $(ANDROID_X86_DIR)/libflex.dylib
ANDROID_X86_SLIB = $(ANDROID_X86_DIR)/libflex.a
ANDROID_X86_CFLAGS = --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86 -mno-sse -mno-mmx -no-canonical-prefixes -msoft-float -target i686-none-linux-android -gcc-toolchain $(ANDROID_NDK)/toolchains/x86-4.9/prebuilt/darwin-x86_64 -MMD -MP -fpic -ffunction-sections -fstack-protector -fno-strict-aliasing -DANDROID -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86/usr/include -Wformat -Werror=format-security

ANDROID_ARM_DIR = $(ANDROID_BUILD_DIR)/armeabi
ANDROID_ARM_FLEX_O = $(ANDROID_ARM_DIR)/flex.o
ANDROID_ARM_DYLIB = $(ANDROID_ARM_DIR)/libflex.dylib
ANDROID_ARM_SLIB = $(ANDROID_ARM_DIR)/libflex.a
ANDROID_ARM_CFLAGS = --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm -no-canonical-prefixes -target armv5te-none-linux-androideabi -march=armv5te -mthumb -msoft-float -marm -gcc-toolchain $(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64 -mtune=xscale -MMD -MP -fpic -ffunction-sections -fstack-protector -fno-strict-aliasing -DANDROID -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm/usr/include -Wformat -Werror=format-security 

all: macos ios android
macos:  $(MACOS_DYLIB) $(MACOS_SLIB)
ios: $(IOS_DYLIB) $(IOS_SLIB)
android: $(ANDROID_X86_DYLIB) $(ANDROID_X86_SLIB) $(ANDROID_ARM_DYLIB) $(ANDROID_ARM_SLIB)

$(MACOS_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(MACOS_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(MACOS_CFLAGS) $(MACOS_ARCHFLAG) -c $(FLEX_SRC) -o $(MACOS_FLEX_O)

$(MACOS_DYLIB): $(MACOS_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(MACOS_CFLAGS) $(MACOS_ARCHFLAG) $(MACOS_FLEX_O) -dynamiclib -undefined dynamic_lookup -o $(MACOS_DYLIB)

$(MACOS_SLIB): $(MACOS_FLEX_O)
	/bin/rm -f $(MACOS_SLIB)
	/usr/bin/ar rcu $(MACOS_SLIB) $(MACOS_FLEX_O)
	/usr/bin/ranlib $(MACOS_SLIB)

$(IOS_SIM_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(IOS_SIM_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(IOS_SIM_CFLAGS) $(IOS_SIM_ARCHFLAG) -c $(FLEX_SRC) -o $(IOS_SIM_FLEX_O)

$(IOS_SIM_DYLIB): $(IOS_SIM_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(IOS_SIM_CFLAGS) $(IOS_SIM_ARCHFLAG) $(IOS_SIM_FLEX_O) -dynamiclib -o $(IOS_SIM_DYLIB)

$(IOS_SIM_SLIB): $(IOS_SIM_FLEX_O)
	/bin/rm -f $(IOS_SIM_SLIB)
	/usr/bin/ar rcu $(IOS_SIM_SLIB) $(IOS_SIM_FLEX_O)
	/usr/bin/ranlib $(IOS_SIM_SLIB)

$(IOS_DEV_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(IOS_DEV_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(IOS_DEV_CFLAGS) $(IOS_DEV_ARCHFLAG) -c $(FLEX_SRC) -o $(IOS_DEV_FLEX_O)

$(IOS_DEV_DYLIB): $(IOS_DEV_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(IOS_DEV_CFLAGS) $(IOS_DEV_ARCHFLAG) $(IOS_DEV_FLEX_O) -dynamiclib -o $(IOS_DEV_DYLIB)

$(IOS_DEV_SLIB): $(IOS_DEV_FLEX_O)
	/bin/rm -f $(IOS_DEV_SLIB)
	/usr/bin/ar rcu $(IOS_DEV_SLIB) $(IOS_DEV_FLEX_O)
	/usr/bin/ranlib $(IOS_DEV_SLIB)

$(IOS_DYLIB): $(IOS_SIM_DYLIB) $(IOS_DEV_DYLIB)
	/usr/bin/lipo -create $(IOS_SIM_DYLIB) $(IOS_DEV_DYLIB) -output $(IOS_DYLIB)

$(IOS_SLIB): $(IOS_SIM_SLIB) $(IOS_DEV_SLIB)
	/usr/bin/lipo -create $(IOS_SIM_SLIB) $(IOS_DEV_SLIB) -output $(IOS_SLIB)

$(ANDROID_X86_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_X86_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_X86_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_X86_FLEX_O)

$(ANDROID_ARM_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_ARM_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_ARM_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_ARM_FLEX_O)

$(ANDROID_X86_DYLIB): $(ANDROID_X86_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_X86_CFLAGS) $(ANDROID_X86_FLEX_O) -shared -Wl,-soname,libflex.so -o $(ANDROID_X86_DYLIB)

$(ANDROID_X86_SLIB): $(ANDROID_X86_FLEX_O)
	/bin/rm -f $(ANDROID_X86_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_X86_SLIB) $(ANDROID_X86_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_X86_SLIB)

$(ANDROID_ARM_DYLIB): $(ANDROID_ARM_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_ARM_CFLAGS) $(ANDROID_ARM_FLEX_O) -shared -Wl,-soname,libflex.so -o $(ANDROID_ARM_DYLIB)

$(ANDROID_ARM_SLIB): $(ANDROID_ARM_FLEX_O)
	/bin/rm -f $(ANDROID_ARM_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_ARM_SLIB) $(ANDROID_ARM_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_ARM_SLIB)

clean:
	/bin/rm -rf $(BUILD_DIR)
