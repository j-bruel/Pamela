# PAMela

*Project for learnings purpose during the Epitech student years.*

_______

What is a PAM module ?
A pluggable authentication module (PAM) is a mechanism to integrate multiple low-level authentication schemes into a high-level application programming interface (API). It allows programs that rely on authentication to be written independent of the underlying authentication scheme. (Wikipédia)

Well, this project consist to create my own custom PAM volume encryption module. This PAM module was developed in C.

## Prerequisites

You muse have those on your laptop :
* Linux OS

## Compile

You can compile the .so files with the command :

```
make
```

This will generate one .so files :

* lib/pamela.so

## Install / uninstall

You can install / uninstall the library with :

```
sudo make install
sudo make uninstall
```

To be sure if the library is intall, you can run the 'check' command :

```
make check
```

## Clean

You can clean the project with this command :

```
make clean
make fclean
```

## Unit Tests

You can run the units tests with :

```
sudo make test
```
