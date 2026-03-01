# capicxx-detailed-example

## Prerequisites
- [`vsomeip` library](https://github.com/COVESA/vsomeip) &rarr; **choose latest version to avoid incompatability issues.**

## Relevant Links
- [CommonAPI Core Runtime Library](https://github.com/COVESA/capicxx-core-runtime) &rarr; Build with CMake and install under path.
- [CommonAPI SOME/IP Runtime Library](https://github.com/COVESA/capicxx-someip-runtime) &rarr; Build with CMake and install under path.
- [CommonAPI Core Generator](https://github.com/COVESA/capicxx-core-tools/releases/download/3.2.15/commonapi_core_generator.zip) &rarr; Download, extract, and use directly with `.fidl` files.
- [CommonAPI SOME/IP Generator](https://github.com/COVESA/capicxx-someip-tools/releases/download/3.2.15/commonapi_someip_generator.zip) &rarr; Download, extract, and use directly with `.fdepl` files.

## How to generate stub + proxy interfaces?
1. Create a directory to store the generated stuff (name it `app_trial` for example --- it is assumed that the generator executables' folders are located next to the other directories). The directory tree should resemble something like this:
    ```
    ├── commonapi_core_generator
    │   └── commonapi-core-generator-linux-x86_64 (core generator executable)
    ├── commonapi_someip_generator
    │   └──  commonapi-someip-generator-linux-x86_64 (someip generator executable)
    ├── fdepl
    ├── fidl
    │   └── bmw
    │       └── ivi
    ├── app_trial
        ├── proxy
        ├── src-gen
        │   └── v1
        │       └── bmw
        │           └── ivi
        └── stub
    ```
1. `cd` into `app_trial`
1. Using `commonapi_core_generator`:
    ```shell
    ../commonapi_core_generator/commonapi-core-generator-linux-x86_64 -sk ../fidl/bmw/ivi/ivi.fidl
    ```
1. Using `commonapi_someip_generator`:
    ```shell
    ../commonapi_someip_generator/commonapi-someip-generator-linux-x86_64 ../fdepl/ivi.fdepl
    ```
1. The two interfaces of interest are `StubDefault.hpp` and `Proxy.hpp`.

## Building and Running Demo
1. Trigger `cmake` to generate build file then build the project.
  ```
  cmake -S . -B build
  cmake --build build --parallel 12
  ```
1. **Remember to run service first.** Before running either, make sure that the init scripts for both are sourced *in the same directory where the binary will be run.*
  ```bash
  # service terminal
  source init_service.sh
  ./IVI-Service
  ```

  ```bash
  # client terminal
  source init_client.sh
  ./IVI-Client
  ```
> **Note:** in case the client and service will be run on separate machines, do not forget to modify the ip address in `.fdepl` file and regenerate the code + the `.json` files for both client and service + the network interface name in the init script.

## Additional Resources
- [CommonAPI C++ Documentation](https://covesa.github.io/capicxx-core-tools/)
- [CommonAPI C++ SomeIP in 10 minutes](https://github.com/COVESA/capicxx-someip-tools/wiki/CommonAPI-C---SomeIP-in-10-minutes)
- [Loading Bindings And Libraries](https://github.com/COVESA/capicxx-core-tools/wiki/Loading-Bindings-And-Libraries)
