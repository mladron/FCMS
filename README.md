# FCMS
Fingerprint Count Min Sketch (FCMS)

## Description
The goal of Fingerprint Count Min Sketch (FCMS) is to reduce counter overestimation by adding a fingerprint to the counters and use it to check if consecutive increments to a counter belong to the same element. This code is used in the paper "Improving Packet Flow Counting with Fingerprint Counting" by Pedro Reviriego, Jorge Martinez and Salvatore Pontarelli, IEEE Communications Letters (in press).

## Compilation
This Visual Studio C++ project consists of 3 files: CMSF.hpp (declaration file), CMSF.cpp (source file) and main.cpp (test bench)

## Command line arguments
Command line arguments are:
- w: table width, default value is w=1048576
- d: table depth or number of counters checked, default value is d=2
- b: fingerprint bits, default value is 0 for standard Count-Min Sketch, when bits is not zero it indicates the bits used for the fingerprint related to the counter
- c: conservative update {0: normal update, 1: conservative update}, default value is c=0
- u: the fingerprint is used only if the counter is less than the given value, default value is u=0
- f: input keys, default value is 1000_ips.txt

**example: cmsf.exe w=1048576 d=2, b=2, f=1000_ips.txt**

## License

MIT
