# Tagberry Qt5 desktop app

[![Build Status](https://travis-ci.org/tagberry/tagberry-qt.svg?branch=master)](https://travis-ci.org/tagberry/tagberry-qt)

*Work in progress!*

## Dependencies

* C++17 compiler
* CMake >= 3.0
* Qt5 >= 5.9

## Build

```
git clone https://github.com/tagberry/tagberry-qt.git
cd tagberry-qt
mkdir build
cd build
cmake ..
make -j
cd ..
```

## Run

```
./bin/Debug/tagberry-qt
```

## Format code

```
./scripts/format.sh
```

## Code structure

The codebase is divided into four parts:

* **widgets** - A collection of application-specific widgets, supplementing standard Qt widgets. Widgets don't know anything about models and business-logic.

* **models** - A collection of data objects to be displayed and manipulated by user. Models don't know anything about widgets and UI.

* **storage** - Classes to read and write models from a persistent storage.

* **presenters** - Classes that put all these things together. Presenters create the UI using widgets and interconnect models with widgets and storage.

![Code Structure](./diagram.png)

## Authors

See [here](https://github.com/tagberry/tagberry-qt/graphs/contributors).

## License

[GPLv2+](LICENSE)
