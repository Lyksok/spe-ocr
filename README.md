# OCR - Optical Character Recognition for Crossword Puzzles

## About
Developed by the **Organized Chaotic Results** team, a group of four EPITA students from the 2028 graduating class: **Caroline Deliere**, **Fanette Saury**, **Jans Guillopé**, and **Lise Suzanne**. This project, built during our 3rd semester, is designed to solve crossword grids using Optical Character Recognition (OCR) paired with a neural network for accurate character recognition.


## Prerequisites

To compile and run the application, ensure the following dependencies are installed on your system.

### Required Packages
GCC, pkg-config, SDL2, SDL2_image, and GTK+3
Ubuntu/Debian
```sh
sudo apt install -y gcc pkg-config libgtk-3-dev libsdl2-dev libsdl2-image-dev
```
Arch/Manjaro
```sh
sudo pacman -S gcc pkgconf gtk3 sdl2 sdl2_image
```
NixOS
```sh
sudo nix-env -iA nixos.gcc nixos.pkgconfig nixos.gtk3 nixos.SDL2 nixos.SDL2_image
```
## Usage
### GUI
```sh
make
./app_launcher
```
### Solver
```sh
cd source/solver
make
./solver <grid as text> <word>
```
### Neural network
```sh
cd source/neural_network
make
./neural
```
### Detection and segmentation
```sh
cd source/detection-and-segmentation
make
./det-seg <image path> <detection/segmentation> <chars/grid/list/words>
```
### Every function is declared in a header file and defined and commented in the source code

## License
This project is licensed under the MIT License. See the full license text below.
### MIT License
Copyright 2024 Organized Chaotic Results

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
