cpp-embedded-rc
===============

`cpp-embedded-rc` or `RC` is a a command line tool and a library to generate
and embed binary resources in your application written in C++.

Install
-------

You will need [conan](https://conan.io) and [cmake](https://cmake.org).

### Conan
The minimal `conanfile.txt` you need looks like:
```
[requires]
cpp-embedded-rc/1.0.0@khdkhd/stable

[generators]
cmake
cmake_paths
```

### CMake
Then your `CMakeLists.txt` should look like:
```cmake
cmake_minimum_required(VERSION 3.0)

project(YOUR_PROJECT CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

conan_basic_setup()

include(rc/generate) # include the function to generate resources

rc_generate(
  ASSETS
  NAME Assets
  FILES
    PATH_TO_ASSET1
    PATH_TO_ASSET2
          ⋮
    PATH_TO_ASSET1
)

add_executable(${PROJECT_NAME}
  main.cpp
  ${ASSETS}
)
target_compile_features(${PROJECT_NAME}
  PRIVATE
    cxx_std_17
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${CONAN_LIBS}
)
```

## Reference

### Generate embeddable resources

#### rc-generate command line tool

##### Usage
```
rc-generate [OPTIONS] file [file...]
```

##### Available options
- `-h`, `--help` - display help message and exit.
- `-v`, `--verbose` - enable verbose mode.
- `-b`, `--base arg` - set base directory value. The default value is the
current working directory.
- `-p`, `--prefix arg` - set base directory value. The default value is `/`.
- `-n`, `--name arg` - set the resource name. The default value is `binary_data`.
- `-d`, `--output-dir arg` - set output directory.

Each created resource is registered with a path.

This path is computed by joining the given prefix with the path of a file
relative to the given base directory.

#### rc_generate CMake function
To be able to create embeddable resources for your application with _CMake_ you
will have to include the module following `rc/generate`.

The easiest way to accomplish this is to use the `conan_paths` _Conan_
generator.

```cmake
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

conan_basic_setup()

include(rc/generate)
```
##### Synopsis
```cmake
rc_generate(OUTPUT_VAR
  [PREFIX prefix_path]
  [BASE basedir_path]
  [NAME name]
  FILES file1 [file2 ...]
)
```

##### Parameters
- `OUTPUT_VAR` - the generated resource file path is stored in the given
variable,
- `PREFIX prefix_path` - the prefix path prepended to each registered resources,
- `BASE basedir_path` - the resource pathes are relative to this path,
- `NAME name` - use the create resource filename, and for `RC_DECLARE` and
`RC_REGISTER` macro.

### Use embedded resources in your code
Before anything make sure you include the rc header.

```cpp
#include <rc.hpp>
```

#### RC_DECLARE
The following code examples assumed some that resources with name
`SomeEmbeddedResources` and `SomeOtherEmbeddedResources` have been generated ()
and linked with your application
(see [Generate embeddable resources](#generate-embeddable-resources) part for
more details).

Use `RC_DECLARE` to declare some resources.

```cpp
#include <rc.hpp>

RC_DECLARE(SomeEmbeddedResources)
RC_DECLARE(SomeOtherEmbeddedResources)
```

#### RC_REGISTER
Once the resources are declared you will be able to register it by calling
`RC_REGISTER` macro to register the declared resources. This step is required
to access resources afterwards.

```cpp
int
main(int argc, char **argv) {
  RC_REGISTER(SomeEmbeddedResources);
  RC_REGISTER(SomeOtherEmbeddedResources);

  // ...

  return 0;
}
```


#### khdkhd::rc::exists
It checks if a resource exists for a given path.

##### Synopsis
```cpp
bool exists(const std::string &path);
```

##### Parameters
- `path` - the resource path to test.

##### Returns
- `true` - if and only if a resource has been registered for this path,
- `false` - otherwise


#### khdkhd::rc::resources
It returns the path list of all registered resources.

##### Synopsis
```cpp
std::vector<std::string> resources();
```
##### Returns
A `std::vector<string>`.


#### khdkhd::rc::streambuf
`khdkhd::rc::streambuf` is a `std::basic_streambuf` whose associated character
sequence is an embedded resource.


##### khdkhd::rc::streambuf::open
Opens the resource at the given path.

###### Synopsis
```cpp
khdkhd::rc::streambuf &open(const std::string &path);
```

###### Parameters
- `path` - the path of the resource to be opened.

###### Returns
`*this`.

##### khdkhd::rc::streambuf::is_open
Checks if the most recent call to `open()` succeeded.

###### Synopsis
```cpp
bool is_open() const;
```

###### Returns

#### khdkhd::rc::istream

##### khdkhd::rc::istream::istream
Constructs new resource input stream.

###### Synopsis
```cpp
khdkhd::rc::istream::istream() (1)
khdkhd::rc::istream::istream(const std::string &path) (2)
```

- (1) Default constructor: constructs a stream that is not associated with a
resource.
- (2) First, performs the same steps as the default constructor, then
associates the stream with a resource by calling `khdkhd::rc::istream::open`.

###### Parameters
- `path` - the path of the resource to be opened.

##### khdkhd::rc::istream::open
Opens and associates the resource with given path with the stream.
Calls `setstate(failbit)` on failure.

###### Synopsis
```cpp
khdkhd::rc::istream &khdkhd::rc::istream::open(const std::string &path)
```

###### Parameters
- `path` - the path of the resource to be opened.

###### Returns
`*this`.


##### khdkhd::rc::istream::is_open
Checks if the file stream has an associated resource.

###### Synopsis
```cpp
khdkhd::rc::istream &khdkhd::rc::istream::is_open() const
```

###### Returns
- `true` if the file stream has an associated file,
- `false` otherwise
