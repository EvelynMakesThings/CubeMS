# CubeMS
*No, MS does not stand for Microsoft.*

*CubeMS* is a replacement for the master server of the game Cube (2001) by Wouter van Oortmerssen.

For quite some time now, the master server for this game has been down, meaning that builds of the game can not see pubic servers. As of the start of the project, the source for the original master server has not been released, thus the existance of this project.

## Dependancies
GNU Autotools and GCC. If you don't have these I'm disappointed in you.

## Building
To build the project, all you have to do is run the following.
```
./autogen.sh
./configure
make
```
If you wish to change the directory that make install puts your files, you can instead run `./configure --prefix=/wherever/`. *(default is /usr/local/)*
