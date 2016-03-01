# NES Scripts

This repository contains a collection of scripts for running and communicating with various NES games over a C++ interface. It aims to give the machine learning community a streamlined tool to get information and controls about the game state of an NES game.

The repository contains basicaly two modules. The communication module used to communicate with the emulator over a C++ interface and the emulator module, that runs the NES game and provides an interface to get various information about the game state.

## Requirements

The communication module has the following dependencies:

```
mlpack
OpenCV
Boost (asio, property_tree) - libboost-all-dev contains all necessary packages
CMake         >= 2.8.5
```

The emulator module has the following dependencies:

```
fceux
lua-cjson
luasocket
lua-gd
```

## Building the communication module from source

The communication module uses CMake as a build system and allows several flexible build configuration options. This tutorial should be enough to get the communication module built.

First, clone the git repository and change into the created directory. Then, make a build directory.

```
$ mkdir build
$ cd build
```

The next step is to run CMake to configure the project. Once CMake is configured, build the communication module.

```
$ cmake ../
$ make
```

## Running the communication module.

After building the communication module, the executable (´´´nes´´´) will reside in build/. You can call them from there, or you can install the executable and (depending on system settings) it should be added to your PATH and you can call them directly. The communication module requires two parameters the IP address or a host name and the port of the machine that runs the emulator module.

We have a running emulator module you can use. Note that the emulation module allows only one connection at a time. Maybe you have to wait, to use the remote emulator module.

```
./nes mario.urgs.org 4561
```

If you have your own emulator module replace the hostname and the port with the specification you used to run the emulator module.

The following list contains the implemented commands you can use to communicate with the emulator module.

| Command  |  Message/Function        | Effect                                                           |
| :-------:|:------------------------:| :---------------------------------------------------------------:|
| a        | messages::PressLeft()    | Move left                                                        |
| d        | messages::PressRight()   | Move right                                                       |
| s        | messages::PressDown()    | Move down                                                        |
| f        | messages::PressA()       | Press the A button (the remote emulator mapped that key to jump) |
| j        | messages::PressB()       | Press the B button (the remote emulator doesn't use that key)    |
| w        | messages::PressUp()      | Move up                                                          |
| k        | messages::PressStart()   | Press the Start button                                           |
| r        | messages::GameReset()    | Reset the game  start from the beginning                         |
| i        | messages::GameImage()    | Get an image of the game state as jpeg                           |
| c        | messages::ConfigFrame()  | Set the number of frames before the next interaction to 30       |
| g        | messages::GameInfo()     | Get all game state informations including the tiles              |

The following contains some example outputs of the game state and the corresponding matrix representation of the game state. The postion of mario is marked with 3, an enemy is marked with 2 and other blocks are marked with 1.

### Example 1

```
{"tiles":{"1":[0,0,0,0,0,0,3,0,0,0,0,0,0],"2":[0,0,1,1,1,1,1,1,1,1,1,1,1],"3":[0,0,1,1,1,1,1,1,1,1,1,1,1],"4":[0,0,0,0,0,0,0,0,0,0,0,0,0],"5":[0,0,0,0,0,0,0,0,0,0,0,0,0],"6":[0,0,0,0,0,0,0,0,0,0,0,0,0],"0":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-5":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-6":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-2":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-3":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-1":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-4":[0,0,0,0,0,0,0,0,0,0,0,0,0]},"lives":2,"mario":{"y":192,"x":57},"coins":0}
```

<p align="center"><img src="https://urgs.org/marcus/media/mario_example_1.png" alt="Mario game state 1" width="400"/></p>

```
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   3   0   0   0   0   0   0
   0   0   1   1   1   1   1   1   1   1   1   1   1
   0   0   1   1   1   1   1   1   1   1   1   1   1
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
```

### Example 2

```
{"tiles":{"1":[0,0,2,0,0,0,3,0,0,0,0,0,0],"2":[1,1,1,1,1,1,1,1,1,1,1,1,1],"3":[1,1,1,1,1,1,1,1,1,1,1,1,1],"4":[0,0,0,0,0,0,0,0,1,1,0,0,0],"5":[0,0,0,0,0,0,0,0,0,0,0,0,0],"6":[0,0,0,0,0,0,0,0,0,0,0,0,0],"0":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-5":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-6":[0,0,0,0,0,0,0,0,0,0,0,0,1],"-2":[0,0,0,0,0,0,1,0,0,0,1,1,1],"-3":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-1":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-4":[0,0,0,0,0,0,0,0,0,0,0,0,0]},"lives":2,"mario":{"y":192,"x":248},"coins":1}
```

<p align="center"><img src="https://urgs.org/marcus/media/mario_example_2.png" alt="Mario game state 1" width="400"/></p>


```
   0   0   0   0   0   0   0   0   0   0   0   0   1
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   1   0   0   0   1   1   1
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   3   0   0   0   2   0   0
   1   1   1   1   1   1   1   1   1   1   1   1   1
   1   1   1   1   1   1   1   1   1   1   1   1   1
   0   0   0   0   0   0   0   0   1   1   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
```

### Example 3

```
{"tiles":{"1":[0,0,0,0,0,0,3,1,1,0,0,0,0],"2":[1,1,1,1,1,1,1,1,1,1,1,1,1],"3":[1,1,1,1,1,1,1,1,1,1,1,1,1],"4":[0,0,0,0,0,0,1,1,1,0,0,0,0],"5":[0,0,1,0,0,0,0,0,0,0,0,0,0],"6":[0,0,0,0,1,0,0,0,0,0,0,0,0],"0":[0,0,0,0,0,0,0,1,1,0,0,0,0],"-5":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-6":[0,1,0,0,0,0,0,0,0,0,0,0,0],"-2":[1,1,1,1,0,0,0,0,0,0,0,0,0],"-3":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-1":[0,0,0,0,0,0,0,0,0,0,0,0,0],"-4":[0,0,0,0,0,0,0,0,0,0,0,0,0]},"lives":2,"mario":{"y":192,"x":434},"coins":1}
```

<p align="center"><img src="https://urgs.org/marcus/media/mario_example_3.png" alt="Mario game state 1" width="400"/></p>

```
   0   1   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   1   1   1   1   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   3   1   1   0   0   0   0
   1   1   1   1   1   1   1   1   1   1   1   1   1
   1   1   1   1   1   1   1   1   1   1   1   1   1
   0   0   0   0   0   0   1   1   1   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
   0   0   0   0   0   0   0   0   0   0   0   0   0
```

## Running the emulator module.

After the dependencies for the emulator module are installed you can run the module.

1. Go into the SuperMarioBros folder and open fceux.
2. Click File -> Load Lua Script -> super_mario_bros.lua
3. Click File -> Open ROM -> Super Mario Bros. (Japan, USA).nes (wait a couple of seconds for mario to be standing on the ground)
4. You should now be able to connect to the emulator module using the communication module using the host and port (defautlt 4561) used to start the emulation module.
