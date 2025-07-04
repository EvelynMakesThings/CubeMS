# CubeMS
*No, MS does not stand for Microsoft.*

*CubeMS* is a replacement for the master server of the game Cube (2001) by Wouter van Oortmerssen.

For quite some time now, the master server for this game has been down, meaning that builds of the game can not see pubic servers. As of the start of the project, the source for the original master server has not been released, thus the existance of this project.

## Installation
This project is currently only on the aur, so refer to the Building section if you are on any other operating system.
If you are on Arch Linux, make sure an aur package manager and install the package `cubems`.
After building from source on other distros, you may run `sudo make install`.

## Usage
`sudo CubeMS` should be all you need to start running the server, and it will run on port 80, accessable at the base path.
You can connect to it by providing the option `-mlocalhost/` on your cube client, or you may replace localhost with wherever you are running the server.
This program also works well through a reverse proxy, using the http header X-Forwarded-For, which you can setup via NGINX. It is recommended to run it through a proxy with a custom base path (PROXY MUST SUPPORT HTTP ON PORT 80.)

As an example, you can run `sudo CubeMS -p 8080 -b /cube/`, which will run on port 8080, and you must run it through a reverse proxy to get port 8080 on port 80 for the clients to access. Then you can start your servers and clients with the options `-mexample.com/cube/`.

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
