# OpenTimer
Fast and minimal speedcubing timer written in Qt.

![image](https://user-images.githubusercontent.com/59435262/171844291-b8bfca08-b037-4695-9f4b-704ee98904d2.png)

# Table of contents
1. [Features](#Features)
2. [Building from source](#build)
  1. [On Linux](#linux_build)

## Features <a name="Features"></a>
- Written using C++ and Qt so it's **blazingly fast**
- Code is open and minimal so **anyone can work on it**
- User Interface made of dock widgets which provides opportunities for **customisation**
- The ability to **load and save** solving sessions on the disk
- **Scramble generator** included
- **Scramble previewer** included

## Building from source <a name="build"></a>
### Dependencies <a name="deps"></a>
- Qt5
- libUSB
- make
### On Linux <a name="linux_build"></a>
1. Clone this repo:
```bash
git clone https://github.com/sewe2000/opentimer
```
2. Change working directory
```bash
cd opentimer
```
3. Configure and compile:
```bash
make clean
qmake -config release
make
```
4.  Run opentimer:
```bash
./speedcubing-timer
```
