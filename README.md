# flex

flex is a flexible box layout system following the <a href="https://www.w3.org/TR/css-flexbox-1/">CSS flexbox module</a> specifications. The goal is to offer a fully compliant implementation with a small and maintainable code base under a permissive license.

flex exposes a plain C API with the same parameters that you would use in CSS to customize the layout of a flexible view hierarchy. The API is designed to be easily interoperable with foreign runtimes (ex. C#) and meant to be used by widget toolkits as the foundation of a view layout API.

flex supports both single and multiple (wrap) lines layouts.

## Getting Started

Currently the only way to use it is to copy the `flex.c` and `flex.h` files to your project. Bindings will be written in a later stage for non-C languages.

## Demo App

Under the `demo` directory you will find an Xcode project that will build a Mac demo app. The app exposes the entire set of flexbox parameters that are implemented and lets you create views (including nested ones), similar to how you would build a more realistic user interface in practice.

## Implementation Status

| Attribute | Status |
|---|---|
| width, height | :ok: |
| padding | :ok: |
| margin | :ok: |
| justify_content flex_start | :ok: |
| justify_content flex_end | :ok: |
| justify_content center | :ok: |
| justify_content stretch | :x: |
| justify_content space_around | :ok: |
| justify_content space_between | :ok: |
| justify_content space_evenly | :ok: |
| align_content flex_start | :x: |
| align_content flex_end | :x: |
| align_content center | :x: |
| align_content stretch | :x: |
| align_content space_around | :x: |
| align_content space_between | :x: |
| align_content space_evenly | :x: |
| align_items flex_start | :ok: |
| align_items flex_end | :ok: |
| align_items center | :ok: |
| align_items stretch | :ok: |
| align_self flex_start | :ok: |
| align_self flex_end | :ok: |
| align_self center | :ok: |
| align_self stretch | :ok: |
| position relative | :ok: |
| position absolute | :x: |
| direction column | :ok: |
| direction column_reverse | :ok: |
| direction row | :ok: |
| direction row_reverse | :ok: |
| wrap no_wrap | :ok: |
| wrap wrap | :ok: |
| wrap wrap_reverse | :x: |
| grow | :ok: |
| shrink | :ok: |
| order | :ok: |
| basis | :ok: |

## Tests

There is a test suite that you can run with the following command:

```
$ make test
```

The test suite doesn't provide full coverage at this stage (as many parameters are in play) and needs more work.
