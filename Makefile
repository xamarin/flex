BASE_NAME = flex
FLEX_SRC = flex.c
FLEX_FILES = flex.c flex.h
BUILD_DIR = ./build

CFLAGS = -Wall -Werror -O3
LDFLAGS = 

MACOS_MIN_TARGET = '10.10'
IOS_MIN_TARGET = '7.0'
ANDROID_API = '21'

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
MACOS_DYLIB = $(MACOS_BUILD_DIR)/lib$(BASE_NAME).dylib
MACOS_SLIB = $(MACOS_BUILD_DIR)/lib$(BASE_NAME).a
MACOS_CFLAGS = -mmacosx-version-min=$(MACOS_MIN_TARGET) -arch i386 -arch x86_64

IOS_BUILD_DIR = $(BUILD_DIR)/ios
IOS_DYLIB = $(IOS_BUILD_DIR)/lib$(BASE_NAME).dylib
IOS_SLIB = $(IOS_BUILD_DIR)/lib$(BASE_NAME).a
IOS_FRAMEWORK = $(IOS_BUILD_DIR)/$(BASE_NAME).framework
IOS_FRAMEWORK_ZIP = $(IOS_FRAMEWORK).zip

IOS_SIM_PNAME = iPhoneSimulator
IOS_SIM_FLEX_O = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/flex.o
IOS_SIM_DYLIB = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/lib$(BASE_NAME).dylib
IOS_SIM_SLIB = $(IOS_BUILD_DIR)/$(IOS_SIM_PNAME)/lib$(BASE_NAME).a
IOS_SIM_CFLAGS = -isysroot $(XCODE_DIR)/Contents/Developer/Platforms/$(IOS_SIM_PNAME).platform/Developer/SDKs/$(IOS_SIM_PNAME).sdk -mios-simulator-version-min=$(IOS_MIN_TARGET) -arch i386 -arch x86_64

IOS_DEV_PNAME = iPhoneOS
IOS_DEV_FLEX_O = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/flex.o
IOS_DEV_DYLIB = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/lib$(BASE_NAME).dylib
IOS_DEV_SLIB = $(IOS_BUILD_DIR)/$(IOS_DEV_PNAME)/lib$(BASE_NAME).a
IOS_DEV_CFLAGS = -isysroot $(XCODE_DIR)/Contents/Developer/Platforms/$(IOS_DEV_PNAME).platform/Developer/SDKs/$(IOS_DEV_PNAME).sdk -mios-version-min=$(IOS_MIN_TARGET) -arch armv7 -arch armv7s -arch arm64 

ANDROID_BUILD_DIR = $(BUILD_DIR)/android
ANDROID_CFLAGS = -no-canonical-prefixes -msoft-float -MMD -MP -fpic -ffunction-sections -fstack-protector -fno-strict-aliasing -DANDROID -Wformat -Werror=format-security

ANDROID_X86_DIR = $(ANDROID_BUILD_DIR)/x86
ANDROID_X86_FLEX_O = $(ANDROID_X86_DIR)/flex.o
ANDROID_X86_DYLIB = $(ANDROID_X86_DIR)/lib$(BASE_NAME).so
ANDROID_X86_SLIB = $(ANDROID_X86_DIR)/lib$(BASE_NAME).a
ANDROID_X86_CFLAGS = $(ANDROID_CFLAGS) --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86 -mno-sse -mno-mmx -target i686-none-linux-android -gcc-toolchain $(ANDROID_NDK)/toolchains/x86-4.9/prebuilt/darwin-x86_64 -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86/usr/include

ANDROID_X86_64_DIR = $(ANDROID_BUILD_DIR)/x86_64
ANDROID_X86_64_FLEX_O = $(ANDROID_X86_64_DIR)/flex.o
ANDROID_X86_64_DYLIB = $(ANDROID_X86_64_DIR)/lib$(BASE_NAME).so
ANDROID_X86_64_SLIB = $(ANDROID_X86_64_DIR)/lib$(BASE_NAME).a
ANDROID_X86_64_CFLAGS = $(ANDROID_CFLAGS) --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86_64 -target x86_64-none-linux-android -gcc-toolchain $(ANDROID_NDK)/toolchains/x86_64-4.9/prebuilt/darwin-x86_64 -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-x86_64/usr/include

ANDROID_ARM_DIR = $(ANDROID_BUILD_DIR)/armeabi
ANDROID_ARM_FLEX_O = $(ANDROID_ARM_DIR)/flex.o
ANDROID_ARM_DYLIB = $(ANDROID_ARM_DIR)/lib$(BASE_NAME).so
ANDROID_ARM_SLIB = $(ANDROID_ARM_DIR)/lib$(BASE_NAME).a
ANDROID_ARM_CFLAGS = $(ANDROID_CFLAGS) --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm -target armv5te-none-linux-androideabi -march=armv5te -mthumb -marm -gcc-toolchain $(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64 -mtune=xscale -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm/usr/include

ANDROID_ARM7_DIR = $(ANDROID_BUILD_DIR)/armeabi-v7a
ANDROID_ARM7_FLEX_O = $(ANDROID_ARM7_DIR)/flex.o
ANDROID_ARM7_DYLIB = $(ANDROID_ARM7_DIR)/lib$(BASE_NAME).so
ANDROID_ARM7_SLIB = $(ANDROID_ARM7_DIR)/lib$(BASE_NAME).a
ANDROID_ARM7_CFLAGS = $(ANDROID_CFLAGS) --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm -target armv7-none-linux-androideabi14 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -gcc-toolchain $(ANDROID_NDK)/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64 -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm/usr/include

ANDROID_ARM64_DIR = $(ANDROID_BUILD_DIR)/arm64-v8a
ANDROID_ARM64_FLEX_O = $(ANDROID_ARM64_DIR)/flex.o
ANDROID_ARM64_DYLIB = $(ANDROID_ARM64_DIR)/lib$(BASE_NAME).so
ANDROID_ARM64_SLIB = $(ANDROID_ARM64_DIR)/lib$(BASE_NAME).a
ANDROID_ARM64_CFLAGS = $(subst -msoft-float,,$(ANDROID_CFLAGS)) --sysroot=$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm64 -target aarch64-none-linux-android -gcc-toolchain $(ANDROID_NDK)/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64 -I$(ANDROID_NDK)/platforms/android-$(ANDROID_API)/arch-arm64/usr/include

all: macos ios android
macos:  $(MACOS_DYLIB) $(MACOS_SLIB)
ios: $(IOS_DYLIB) $(IOS_SLIB) $(IOS_FRAMEWORK_ZIP)
android: $(ANDROID_X86_DYLIB) $(ANDROID_X86_SLIB) $(ANDROID_X86_64_DYLIB) $(ANDROID_X86_64_SLIB) $(ANDROID_ARM_DYLIB) $(ANDROID_ARM_SLIB) $(ANDROID_ARM7_DYLIB) $(ANDROID_ARM7_SLIB) $(ANDROID_ARM64_DYLIB) $(ANDROID_ARM64_SLIB) 

$(MACOS_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(MACOS_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(MACOS_CFLAGS) -c $(FLEX_SRC) -o $(MACOS_FLEX_O)

$(MACOS_DYLIB): $(MACOS_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(MACOS_CFLAGS) $(MACOS_FLEX_O) -dynamiclib -undefined dynamic_lookup -o $(MACOS_DYLIB)

$(MACOS_SLIB): $(MACOS_FLEX_O)
	/bin/rm -f $(MACOS_SLIB)
	/usr/bin/ar rcu $(MACOS_SLIB) $(MACOS_FLEX_O)
	/usr/bin/ranlib $(MACOS_SLIB)

$(IOS_SIM_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(IOS_SIM_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(IOS_SIM_CFLAGS) -c $(FLEX_SRC) -o $(IOS_SIM_FLEX_O)

$(IOS_SIM_DYLIB): $(IOS_SIM_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(IOS_SIM_CFLAGS) $(IOS_SIM_FLEX_O) -dynamiclib -o $(IOS_SIM_DYLIB)

$(IOS_SIM_SLIB): $(IOS_SIM_FLEX_O)
	/bin/rm -f $(IOS_SIM_SLIB)
	/usr/bin/ar rcu $(IOS_SIM_SLIB) $(IOS_SIM_FLEX_O)
	/usr/bin/ranlib $(IOS_SIM_SLIB)

$(IOS_DEV_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(IOS_DEV_FLEX_O)`
	$(XCODE_CC) $(CFLAGS) $(IOS_DEV_CFLAGS) -c $(FLEX_SRC) -o $(IOS_DEV_FLEX_O)

$(IOS_DEV_DYLIB): $(IOS_DEV_FLEX_O)
	$(XCODE_CC) $(LDFLAGS) $(IOS_DEV_CFLAGS) $(IOS_DEV_FLEX_O) -dynamiclib -o $(IOS_DEV_DYLIB)

$(IOS_DEV_SLIB): $(IOS_DEV_FLEX_O)
	/bin/rm -f $(IOS_DEV_SLIB)
	/usr/bin/ar rcu $(IOS_DEV_SLIB) $(IOS_DEV_FLEX_O)
	/usr/bin/ranlib $(IOS_DEV_SLIB)

$(IOS_DYLIB): $(IOS_SIM_DYLIB) $(IOS_DEV_DYLIB)
	/usr/bin/lipo -create $(IOS_SIM_DYLIB) $(IOS_DEV_DYLIB) -output $(IOS_DYLIB)

$(IOS_SLIB): $(IOS_SIM_SLIB) $(IOS_DEV_SLIB)
	/usr/bin/lipo -create $(IOS_SIM_SLIB) $(IOS_DEV_SLIB) -output $(IOS_SLIB)

$(IOS_FRAMEWORK_ZIP): $(IOS_SLIB)
	/bin/rm -rf $(IOS_FRAMEWORK)
	/bin/mkdir -p $(IOS_FRAMEWORK)
	/bin/cp flex.framework.plist $(IOS_FRAMEWORK)/Info.plist
	/bin/cp $(IOS_DYLIB) $(IOS_FRAMEWORK)/flex
	/usr/bin/install_name_tool -id @rpath/flex.framework/flex $(IOS_FRAMEWORK)/flex
	(cd `/usr/bin/dirname $(IOS_FRAMEWORK_ZIP)` && /usr/bin/zip -r `/usr/bin/basename $(IOS_FRAMEWORK_ZIP)` `/usr/bin/basename $(IOS_FRAMEWORK)`)

$(ANDROID_X86_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_X86_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_X86_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_X86_FLEX_O)

$(ANDROID_X86_64_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_X86_64_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_X86_64_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_X86_64_FLEX_O)

$(ANDROID_ARM_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_ARM_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_ARM_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_ARM_FLEX_O)

$(ANDROID_ARM7_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_ARM7_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_ARM7_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_ARM7_FLEX_O)

$(ANDROID_ARM64_FLEX_O): $(FLEX_FILES)
	/bin/mkdir -p `/usr/bin/dirname $(ANDROID_ARM64_FLEX_O)`
	$(ANDROID_CC) $(CFLAGS) $(ANDROID_ARM64_CFLAGS) -c $(FLEX_SRC) -o $(ANDROID_ARM64_FLEX_O)

$(ANDROID_X86_DYLIB): $(ANDROID_X86_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_X86_CFLAGS) $(ANDROID_X86_FLEX_O) -shared -Wl,-soname,`/usr/bin/basename $(ANDROID_X86_DYLIB)` -o $(ANDROID_X86_DYLIB)

$(ANDROID_X86_SLIB): $(ANDROID_X86_FLEX_O)
	/bin/rm -f $(ANDROID_X86_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_X86_SLIB) $(ANDROID_X86_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_X86_SLIB)

$(ANDROID_X86_64_DYLIB): $(ANDROID_X86_64_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_X86_64_CFLAGS) $(ANDROID_X86_64_FLEX_O) -shared -Wl,-soname,`/usr/bin/basename $(ANDROID_X86_64_DYLIB)` -o $(ANDROID_X86_64_DYLIB)

$(ANDROID_X86_64_SLIB): $(ANDROID_X86_64_FLEX_O)
	/bin/rm -f $(ANDROID_X86_64_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_X86_64_SLIB) $(ANDROID_X86_64_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_X86_64_SLIB)

$(ANDROID_ARM_DYLIB): $(ANDROID_ARM_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_ARM_CFLAGS) $(ANDROID_ARM_FLEX_O) -shared -Wl,-soname,`/usr/bin/basename $(ANDROID_ARM_DYLIB)` -o $(ANDROID_ARM_DYLIB)

$(ANDROID_ARM_SLIB): $(ANDROID_ARM_FLEX_O)
	/bin/rm -f $(ANDROID_ARM_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_ARM_SLIB) $(ANDROID_ARM_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_ARM_SLIB)

$(ANDROID_ARM7_DYLIB): $(ANDROID_ARM7_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_ARM7_CFLAGS) $(ANDROID_ARM7_FLEX_O) -shared -Wl,-soname,`/usr/bin/basename $(ANDROID_ARM7_DYLIB)` -o $(ANDROID_ARM7_DYLIB)

$(ANDROID_ARM7_SLIB): $(ANDROID_ARM7_FLEX_O)
	/bin/rm -f $(ANDROID_ARM7_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_ARM7_SLIB) $(ANDROID_ARM7_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_ARM7_SLIB)

$(ANDROID_ARM64_DYLIB): $(ANDROID_ARM64_FLEX_O)
	$(ANDROID_CC) $(LDFLAGS) $(ANDROID_ARM64_CFLAGS) $(ANDROID_ARM64_FLEX_O) -shared -Wl,-soname,`/usr/bin/basename $(ANDROID_ARM64_DYLIB)` -o $(ANDROID_ARM64_DYLIB)

$(ANDROID_ARM64_SLIB): $(ANDROID_ARM64_FLEX_O)
	/bin/rm -f $(ANDROID_ARM64_SLIB)
	$(ANDROID_BIN_DIR)/ar rcu $(ANDROID_ARM64_SLIB) $(ANDROID_ARM64_FLEX_O)
	$(ANDROID_BIN_DIR)/ranlib $(ANDROID_ARM64_SLIB)

clean:
	/bin/rm -rf $(BUILD_DIR)
