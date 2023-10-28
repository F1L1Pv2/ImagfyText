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
.___                        _____       ___________              __
|   | _____ _____     _____/ ____\__.__.\__    ___/___ ___  ____/  |_
|   |/     \\__  \   / ___\   __<   |  |  |    |_/ __ \\  \/  /\   __\
|   |  Y Y  \/ __ \_/ /_/  >  |  \___  |  |    |\  ___/ >    <  |  |
|___|__|_|  (____  /\___  /|__|  / ____|  |____| \___  >__/\_ \ |__|
          \/     \//_____/       \/                  \/      \/
                                                      Made By: F1L1Pv2
[Usage] Modes "encode", "decode"
[Usage]       "encode" + <filepath> + <image_width>
[Usage]       "decode" + <filepath> + <decode_flags>
[Note]        <filepath> needs to exist
[Usage] Encode flags: "background_img"
[Usage]               "background_img" - this will be an image data that will be written on
[Note]                after "background_img" you need to pass additional filepath
[Usage] Decode flags: "out_show", "out_write"
[Usage]               "out_show" - file output will be printed into stdout
[Usage]               "out_write" - file output will be saved into file
[Usage] Global Params: "alpha"
[Usage]                "alpha" - will save bytes only in alpha channel
[Usage] ImagfyText.exe <mode> <additional params> <global params>
[Error] Mode wasn't provided
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
