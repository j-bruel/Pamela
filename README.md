PAMela

Realization of a PAM lib for linux in C.

_____________________

Prerequisites

You muse have those on your laptop :
    * Linux OS

____________________

Compile

You can compile the .so files with the command :

------------------
make
------------------

This will generate one .so files :

    * lib/pamela.so
___________________

Install / uninstall

You can install / uninstall the library with :

-----------------
sudo make install
sudo make uninstall
-----------------

To be sure if the library is intall, you can run the 'check' command :

-----------
make check
-----------

___________________

Clean

You can clean the project with this command :

-----------------
make clean
make fclean
-----------------

___________________

Unit Tests

You can run the units tests with :

-----------------
sudo make test
-----------------
