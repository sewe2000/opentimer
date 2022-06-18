![image](https://shields.io/tokei/lines/github/sewe2000/opentimer)
![image](https://shields.io/github/downloads/sewe2000/opentimer/total)
![image](https://shields.io/github/license/sewe2000/opentimer)
![image](https://shields.io/aur/version/opentimer)
![image](https://shields.io/github/v/release/sewe2000/opentimer?display_name=tag&include_prereleases)

# OpenTimer
Fast and minimal speedcubing timer written in Qt.

![image](https://user-images.githubusercontent.com/59435262/171844291-b8bfca08-b037-4695-9f4b-704ee98904d2.png)

# Table of contents
1. [Features](#Features)
2. [Installation](#installation)
   + [For Microsoft Windows](#install_windows)
   + [For Linux](#linux-install)
      - [Arch-based distros](#aur-install)
3. [Building from source](#build)
   + [On Linux](#linux_build)


## Features <a name="Features"></a>
- Written using C++ and Qt so it's **blazingly fast**
- Code is open and minimal so **anyone can work on it**
- User Interface made of dock widgets which provides opportunities for **customisation**
- The ability to **load and save** solving sessions on the disk
- It is **Libre/Free software** under the GPL v3 License 
- Does not **spy on you**
- You can use it on both **Windows and Linux**
- Supports 32-bit Windows machines so you can use it on **older hardware**
- **Scramble generator** included
- **Scramble previewer** included

## Installation <a name="installation"></a>
### For Microsoft Windows <a name="install_windows"></a>
Go to the [Release page](https://github.com/sewe2000/opentimer/releases) of this project and grab 'opentimer-installer-x86.exe' file, open it and follow the instructions.

### For Linux <a name="linux-install"></a>
#### Arch-based distros <a name="aur-install"></a>
On Arch based distributions you can just download it from the AUR. For example if you're using yay:
```
yay -S opentimer
```

## Building from source <a name="build"></a>
### Dependencies <a name="deps"></a>
- Qt5
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
qmake -config release
make
```
4.  Run opentimer:
```bash
./opentimer
```
