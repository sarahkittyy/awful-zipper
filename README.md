# awful-zipper

The worst zipper in the world

## Features

* Multiple ugly nested folders / zips
* Supports zipping directories and files
* Randomly generated intermediate zip names
* Randomly password-protected intermediate steps, where the password is one in a text file full of 6-8 passwords

## Dependencies

* [SFML 2.5](https://www.sfml-dev.org/download.php)
* [zlib](http://www.zlib.net/)

## Building

```bash
git clone https://github.com/sarahkittyy/awful-zipper.git
cd awful-zipper
git submodule update --init --recursive
cd build
cmake ..
make
```

## Usage

```bash
cd awful-zipper
./build/awful-zipper
```