# Xamarin.Flex

This directory contains the C# bindings for the Flex library. The bindings are meant to be used by Xamarin projects (iOS, Android, macOS, Forms, etc.).

## Getting Started

The best way to get started with Xamarin.Flex is to install the NuGet package:

https://www.nuget.org/packages/Xamarin.Flex

In a Xamarin project, this can done in the Packages folder of your Visual Studio solution.

## Building the Bindings

You may want to build your own version of the bindings, in case you would like to use changes that haven't been released yet, or testing changes you made in your local copy.

The first thing to do is to build the native code for all the platforms that you might need. You can simply run `make` in the root project directory (you may have to tweak the `Makefile` according to your environment):

```
$ (cd ../.. && make)
```

Now that the native code is built, you can build the C# bindings. Running `make` in this directory will build the C# code into a DLL file and package it along with the native code to create a NuGet package:

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
$ open en.html/index.html
```

The documentation is also available online here:

https://developer.xamarin.com/api/namespace/Xamarin.Flex

## Samples

You will find under the `samples` directory a few projects using the bindings. These projects require the Xamarin.Flex NuGet package.

## Tests

The bindings are covered by tests. The tests specifically cover logic added by the bindings themselves, the native test suite (in the `tests` directory of the root project) covers the flexbox implementation itself.

### Running the tests on a Mac

```
$ cd tests
$ make
```

### Running the tests on Windows

```
$ cd tests
$ msbuild
$ bin\x64\Debug\test.exe
```
