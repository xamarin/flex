# flex

flex is a flexible box layout system inspired from the <a href="https://www.w3.org/TR/css-flexbox-1/">CSS flexbox module</a>.

Its goal is to provide a plain C API that exposes similar parameters that can be used to customize the layout of a view hierarchy. The API is designed to be easily interoperable with foreign runtimes (ex. C#).

## Getting started

Currently the only way to use it is to copy the `flex.c` and `flex.h` files to your project. Bindings will be written in a later stage for non-C languages.

## Demo app

Under the `demo` directory you will find an Xcode project that will build a Mac demo app. The app exposes the entire set of flexbox parameters and lets you create children (including nested ones), similar to how you would build a more realistic user interface.

## Status

| Feature | Status |
|---|---|
| width, width | :ok: |
| padding | :ok: |
| margin | :ok: |
| justify_content | :ok: |
| align_content | :x: |
| align_items | :ok: |
| align_self | :ok: |
| position relative | :ok: |
| position absolute | :x: |
| direction | :ok: |
| wrap | :ok: |
| grow | :ok: |
| shrink | :ok: |
| order | :ok: |
| basis | :ok: |

## Tests

There is a test suite that you can run with the following command:

```
$ make test
```

However it's not that great yet.
