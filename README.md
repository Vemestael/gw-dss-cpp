# gw-dss-cpp

gw-dss-cpp - simple tool that use queueing theory concepts to make predictions about needed number of technical support staff. 

## Build

1. First of all make sure you have installed successfully:
* Qt Framework (Qt 5 or Qt 6) (see more [Open Source Qt Use](https://www.qt.io/download-open-source));
* Boost library (see more [Boost installation on Windows](https://www.boost.org/doc/libs/1_75_0/more/getting_started/windows.html) or [Boost installation on *nix](https://www.boost.org/doc/libs/1_75_0/more/getting_started/unix-variants.html)).

2. Clone this project and then cd to the project folder;

3. Init the git submodule:
```
$ git submodule init
$ git submodule update --recursive
$ git submodule foreach --recursive git submodule update --init 
```

4. Configure the project using CMake:
```
$ cmake -B ./build -G <preferred generator>
```

5. Then run build command:
```
$ cmake --build ./build --config <Debug|Release> --target <gw-dss-cpp|deployment|all>
```
> As you can see, there is two available targets:
> * gw-dss-cpp - build application independently:
> * deployment - run deployment(only on Windows).

6. You've done! The builded binary file(s) available in the build directory.

## Usage

Coming soon

## License

This project is licensed under the [MIT License](LICENSE).

## Credits

* [Qt Framework](https://github.com/qt) by [The Qt Company](https://www.qt.io/)
* [qtm-calc](https://github.com/andinoriel/qtm-calc/) by [andinoriel](https://github.com/andinoriel/)
