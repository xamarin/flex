# flex

flex is a flexible box layout system following the <a href="https://www.w3.org/TR/css-flexbox-1/">CSS flexbox module</a> specifications. The goal is to offer a fully compliant implementation with a small and maintainable code base under a permissive license.

flex exposes a plain C API with the same parameters that you would use in CSS to customize the layout of a flexible view hierarchy. The API is designed to be easily interoperable with foreign runtimes (ex. C#) and meant to be used by widget toolkits as the foundation of a view layout API.

flex supports both single and multiple (wrap) lines layouts.

## Getting Started

If you program in C# you can go straight to the [bindings/csharp](bindings/csharp) directory.

If you program in a C-compatible environment you can simply copy the `flex.c` and `flex.h` files to your project. The code was written to be cross-platform and does not require dependencies.

On a Mac you can also generate static and dynamic libraries for iOS, Android and macOS using `make`:

```
$ make macos
$ make ios
$ make android
$ make             # builds everything
```

Make sure to have the `ANDROID_NDK` environment variable set to the path where the Android NDK is located in your system. You can also tweak build variables by editing the `Makefile` file.

On a Windows machine you can generate dynamic libraires (DLL) for x86, x64, ARM and ARM64 by opening the Visual Studio project file or running `msbuild` from the command line.

## Demo App

Under the `demo` directory you will find an Xcode project that will build a Mac demo app. The app exposes the entire set of flexbox parameters that are implemented and lets you create views (including nested ones), similar to how you would build a more realistic user interface in practice.

## Implementation Status

| Attribute | Status |
|---|---|
| width, height | :ok: |
| self_sizing | :ok: |
| padding | :ok: |
| margin | :ok: |
| justify_content flex_start | :ok: |
| justify_content flex_end | :ok: |
| justify_content center | :ok: |
| justify_content space_around | :ok: |
| justify_content space_between | :ok: |
| justify_content space_evenly | :ok: |
| align_content flex_start | :ok: |
| align_content flex_end | :ok: |
| align_content center | :ok: |
| align_content space_around | :ok: |
| align_content space_between | :ok: |
| align_content space_evenly | :ok: |
| align_content stretch | :ok: |
| align_items flex_start | :ok: |
| align_items flex_end | :ok: |
| align_items center | :ok: |
| align_items stretch | :ok: |
| align_self flex_start | :ok: |
| align_self flex_end | :ok: |
| align_self center | :ok: |
| align_self stretch | :ok: |
| position relative | :ok: |
| position absolute | :ok: |
| direction column | :ok: |
| direction column_reverse | :ok: |
| direction row | :ok: |
| direction row_reverse | :ok: |
| wrap no_wrap | :ok: |
| wrap wrap | :ok: |
| wrap wrap_reverse | :ok: |
| grow | :ok: |
| shrink | :ok: |
| order | :ok: |
| basis | :ok: |

## Tests

There is a test suite in the `tests` directory. See the [tests/README.md](tests/README.md) file for more details on how to build, run and contribute to the test suite.

## License

flex is distributed under the terms of the MIT license. See the `LICENSE.txt` file for more information.
