# Xamarin.Flex

This directory contains the C# bindings for the Flex library. The bindings are meant to be used by Xamarin projects (iOS, Android, macOS, Forms, etc.).

## Building the bindings

The first thing to do is to build the native code for all the platforms that you might need. You can simply run `make` in the root project directory (you may have to tweak the `Makefile` according to your environment):

```
$ (cd ../.. && make)
```

Now that the native code is built, you can build the C# bindings. Running `make` in this directory will build 2 Xamarin.Flex.dll files (one for projects where the native code is statically linked, such as iOS and macOS, and one for projects where the native code is dynamically loaded at runtime, such as Android), then create a NuGet package:

```
$ make
```

This should leave you with a `Xamarin.Flex.???.nupkg` file in this directory that you can add to your Xamarin project. Once added, the API should be available in your project and you should be good to go!

## Documentation

The bindings are fully documented using Mono Documentation XML format. 

You can generate the documentation by running `make` in the `docs` directory which creates HTML files under the `en.html` sub-directory.

```
$ cd docs
$ make
$ open en.html
```

## Samples

You will find under the `samples` directory a few projects using the bindings. These projects require the Xamarin.Flex NuGet package.

## Tests

The bindings are covered by tests. The tests specifically cover logic added by the bindings themselves, the native test suite (in the `tests` directory of the root project) covers the flexbox implementation itself.

```
$ cd tests
$ make
```
