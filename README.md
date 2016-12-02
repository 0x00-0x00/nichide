# NicHide - network interface card anonimity tool [![GitHub release](https://img.shields.io/badge/version-0.11-blue.svg)]
-----

## Project information
NicHide is a security tool to hide NIC's ethernet addresses in linux boxes.
It uses coreutils and other basic linux packages  aside  with  mac  address  generation  algorithm  to
effectively change the hardware signature from the network card,  granting  to  the  user a  new  hard-
ware address to use, causing all the packets sent from it to have this new, randomly generated address.

## Instructions
This section will tutor the user to compile and install the program into the system.

### Compiling and installation
To compile the program, 'make' is required.
The command to do it is the following:

```
make
```

```
make install
```

If the output does not report any problems, the program was successfully installed. To check, type:

```
nichide --help
```

### Usage
The program currently has 3 modes of operation:

#### Random mac
To generate a random mac and set it to a interface named 'eth0', type at terminal:
```
nichide -i eth0 -r
```

#### Original mac
To restore the original (also known as permanent) address of the interface pointed above, type at terminal:
```
nichide -i eth0 -p
```

#### Manual mac
To set an address AA:AA:AA:AA:AA:AA manually to the interface previously mentioned, type at terminal:
```
nichide -i eth0 -m AA:AA:AA:AA:AA:AA
```
