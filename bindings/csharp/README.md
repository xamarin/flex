# Xamarin.Flex

This directory contains the C# bindings for the Flex library. The bindings are meant to be used by Xamarin projects (iOS, Android, macOS, Forms, etc.).

## Building the bindings

The first thing to do is to build the native library for all the platforms that you might need. You can simply run `make` in the root project directory (you may have to tweak the `Makefile` for your environment):

```
$ cd ../..
$ make
$ cd -
```

Now that the native code is built, you can build the C# code. Running `make` in this directory will build the code twice; one for projects where the native library is statically linked, and one for projects where the native library is dynamically loaded at runtime:

```
$ make
```

After this command you can generate the NuGet file by running the `create_nuget.sh` script in the root directory/

```
$ cd ../..
$ sh create_nuget.sh
```

This should leave you with a `Xamarin.Flex.???.nupkg` file that you can add to your Xamarin project, and you should be good to go!

## Documentation

The bindings are fully documented using Mono Documentation XML format. 

You can generate the documentation by running `make` in the `docs` directory which creates HTML files under the `en.html` sub-directory.

```
$ cd docs
$ make
$ open en.html
```

## Tests

The bindings are covered by tests. The tests specifically cover logic added by the bindings themselves, the native test suite (in the `tests` directory of the root project) covers the flexbox implementation itself.

```
$ cd tests
$ make
```
