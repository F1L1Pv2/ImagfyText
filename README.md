# ImagfyText
 Simple utility for turning anything into png and vice versa

# Compilation

## Linux
`g++ -o ImagfyText ImagfyText.cpp`

## Windows

recomended way is to use vs studio sln file

# Usage
```bash
./ImagfyText
[Usage]: Modes: "encode", "decode"
[Usage]:        "encode" + <filepath> + <image_width>
[Usage]:        "decode" + <filepath>
[Usage]: Decode flags: "binary", "out_write"
[Usage]:               "binary" - file will be interpreted as bytecode rather than ascii characters (file wont be printed but it will be saved on drive)
[Usage]:               "out_write" - file output will be saved into file
[Note]:         <filepath> needs to exist
[Usage]: ImagfyText.exe <mode> <additional params>
```
### Here is binary of this utility in its own format

#### Win64
![win_64_binary](https://github.com/F1L1Pv2/ImagfyText/blob/main/ImagfyText_win64.exe.png?raw=true)

#### Win64 (alpha mode)
![win_64_binary](https://github.com/F1L1Pv2/ImagfyText/blob/main/ImagfyText_win64_alpha.exe.png?raw=true)

#### GNU/Linux ELF
![linux_binary](https://github.com/F1L1Pv2/ImagfyText/blob/main/ImagfyText_linux.png?raw=true)

#### GNU/Linux ELF (alpha mode)
![linux_binary](https://github.com/F1L1Pv2/ImagfyText/blob/main/ImagfyText_linux_alpha.png?raw=true)
