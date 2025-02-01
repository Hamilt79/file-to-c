# file-to-c
# Intro
Simple program that converts a file to a c-style header, similar to ```xxd -i```.
# Building/Installation
## Build
```make```
## Build and Install To /usr/bin
```sudo make install```
## Build and Install to /home/<user>/.local/bin
```make install-local```
## Uninstall from /usr/bin
```sudo make uninstall```
## Uninstall from /home/<user>/.local/bin
```make uninstall-local```
# Use
## Non-specific variable name
```f2c input.png output.h```
## Specific variable name
```f2c input.png output.h interesting_var```
