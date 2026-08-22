# Installing and consuming

Back: [izzi](../README.md)

Install the canonical header-only package to a local prefix and consume it
through the exported target:

```sh
cmake -S . -B build/install \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX="$PWD/build/prefix"
cmake --build build/install --target install
```

A downstream CMake project can then use:

```cmake
find_package(izzi 2 CONFIG REQUIRED)
target_link_libraries(my-program PRIVATE izzi::izzi)
```

The default installation contains canonical `izzi-*` headers and the versioned
include fragments. New code should include `izzi-svg.h`. Run
`make check-install` to verify build-tree discovery, relocated canonical
installation, exact inventories, and external consumers.

The graph authoring and resolved-document schemas are installed below
`share/izzi/schemas`. They are versioned independently from the CMake package.
