#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <string>

static enum StringValue {
    OutShow,
    OutWrite,
    OnAlpha,
    BackGroundImg
};

static std::map<std::string, StringValue> FlagsMap;


char* shift_args(int& argc, char**& argv) {
    if (argc > 0) {
        char* arg = *argv;
        argc -= 1;
        argv += 1;
        return arg;
    }
    return nullptr;
}

void title() {
    std::cout << "\033[35m";
    std::cout << ".___                        _____       ___________              __   " << std::endl;
    std::cout << "|   | _____ _____     _____/ ____\\__.__.\\__    ___/___ ___  ____/  |_ " << std::endl;
    std::cout << "|   |/     \\\\__  \\   / ___\\   __<   |  |  |    |_/ __ \\\\  \\/  /\\   __\\" << std::endl;
    std::cout << "|   |  Y Y  \\/ __ \\_/ /_/  >  |  \\___  |  |    |\\  ___/ >    <  |  |  " << std::endl;
    std::cout << "|___|__|_|  (____  /\\___  /|__|  / ____|  |____| \\___  >__/\\_ \\ |__|  " << std::endl;
    std::cout << "          \\/     \\//_____/       \\/                  \\/      \\/       " << std::endl;
    std::cout << "\033[0m";
    std::cout << "\033[33m                                                      Made By: F1L1Pv2\033[0m" << std::endl;
}

void error(const char* reason) {
    std::cout << "\033[31m" << "[Error] " << "\033[0m" << reason << std::endl;
}

void info(const char* info) {
    std::cout << "\033[33m" << "[Info] " << "\033[0m" << info << std::endl;
}

void usage_msg(const char* msg) {
    std::cout << "\033[32m" << "[Usage] " << "\033[0m" << msg << std::endl;
}

void usage_note(const char* note) {
    std::cout << "\033[36m" << "[Note]  " << "\033[0m" << note << std::endl;
}

void usage(const char* program, const char* reason = nullptr) {
    title();
    usage_msg("Modes \"encode\", \"decode\"");
    usage_msg("      \"encode\" + <filepath> + <image_width>");
    usage_msg("      \"decode\" + <filepath> + <decode_flags>");
    usage_note("      <filepath> needs to exist");
    usage_msg("Encode flags: \"background_img\"");
    usage_msg("              \"background_img\" - this will be an image data that will be written on");
    usage_note("              after \"background_img\" you need to pass additional filepath");

    usage_msg("Decode flags: \"out_show\", \"out_write\"");
    usage_msg("              \"out_show\" - file output will be printed into stdout");
    usage_msg("              \"out_write\" - file output will be saved into file");
    usage_msg("Global Params: \"alpha\"");
    usage_msg("               \"alpha\" - will save bytes only in alpha channel");

    std::stringstream msg;
    msg << program << " <mode> <additional params> <global params>";
    usage_msg(msg.str().c_str());
    if (reason != nullptr) error(reason);
}

const unsigned int OUT_SHOW = 1;  //01
const unsigned int OUT_WRITE = 2;    //10
const unsigned int ON_ALPHA = 4;    //100
const unsigned int BACKGROUND_IMG = 8; //1000

void init() {
    FlagsMap["out_show"] = OutShow;
    FlagsMap["out_write"] = OutWrite;
    FlagsMap["alpha"] = OnAlpha;
    FlagsMap["background_img"] = BackGroundImg;
}

int main(int argc, char** argv)
{
    
    init();

    unsigned int flags = 0;

    const char* program = shift_args(argc, argv);

    if (argc <= 0) {
        usage(program, "Mode wasn't provided");
        return -1;
    }


    const char* mode = shift_args(argc, argv);

    std::string mode_str = mode;

    if(mode_str != "encode" && mode_str != "decode" ) {
        std::stringstream error_msg;
        error_msg << "Invalid mode was provided \"";
        error_msg << mode << '\"';
        usage(program, error_msg.str().c_str());
        return -1;
    }

    if (argc <= 0) {
        usage(program, "file path wasn't provided");
        return -1;
    }

    const char* filepath = shift_args(argc, argv);
    char* background_filepath = nullptr;

    const char* str_width = nullptr;

    if (mode_str == "encode") {
        if (argc <= 0) {
            usage(program, "image width wasn't provided");
            return -1;
        }

        str_width = shift_args(argc, argv);

        while (argc > 0) {
            char* flag_chr = shift_args(argc, argv);
            std::string flag;
            flag = flag_chr;

            switch (FlagsMap[flag]) {
            case BackGroundImg:

                if (argc <= 0) {
                    usage("background img width wasn't provided");
                    return -1;
                }

                background_filepath = shift_args(argc, argv);

                flags |= BACKGROUND_IMG;
                break;
            case OnAlpha:
                flags |= ON_ALPHA;
                break;
            default:
                std::stringstream reason;
                reason << "Unknown Encode Flag " << flag;
                error(reason.str().c_str());
                return -1;
            }
        }
    }

    if (mode_str == "decode") {
        while (argc > 0) {
            char* flag_chr = shift_args(argc, argv);
            std::string flag;
            flag = flag_chr;

            switch (FlagsMap[flag]) {

            case OutShow:
                flags |= OUT_SHOW;
                break;
            case OutWrite:
                flags |= OUT_WRITE;
                break;
            case OnAlpha:
                flags |= ON_ALPHA;
                break;
            default:
                std::stringstream reason;
                reason << "Unknown Decode Flag " << flag;
                error(reason.str().c_str());
                return -1;
            }


        }
    }
    
    if (argc > 0) {
        usage(program, "too many arguments were provided");
        return -1;
    }

    if (mode_str == "encode") {

    if(str_width == nullptr) {
        error("Unreachable str_width is nullptr");
        return -1;
    }


    std::ifstream input(filepath, std::ios::binary);

    if (!input.is_open()) {
        error("Input File doesn't exist");
        return -1;
    }

    std::vector<char> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));

    input.close();

    void* data;
    unsigned int width;
    unsigned int height;
    unsigned int pixels_count;


    if ((flags & ON_ALPHA) == 0) {
        while (bytes.size() % 4 != 0) {
            bytes.push_back(0);
        }

        pixels_count = bytes.size() / 4;
    }
    else {
        pixels_count = bytes.size();
    }

    std::stringstream strValue;
    strValue << str_width;

    strValue >> width;

    if (width == NULL) {
        std::stringstream error_msg;
        error_msg << "Invalid Width was provided: " << strValue.str();
        error(error_msg.str().c_str());
        return -1;
    }

    while (pixels_count % width != 0) {
        
        bytes.push_back(0);


        if ((flags & ON_ALPHA) == 0) {
            pixels_count = bytes.size() / 4;
        }
        else {
            pixels_count = bytes.size();
        }

    }

    height = pixels_count / width;

    std::vector<char> buff;

    if ((flags & ON_ALPHA) == 0) {
        data = bytes.data();
    }
    else {

        if ((flags & BACKGROUND_IMG) != 0) {

            int width, height, channels;
            unsigned char* img = stbi_load(background_filepath, &width, &height, &channels, 0);
            if (img == NULL) {
                error("Error in loading the background image\n");
                return -1;
            }

            for (unsigned int i = 0; i < width * height * channels; ++i) {
                buff.push_back(*(img + i));
            }

        }
        else {
            for (unsigned int i = 0; i < width * height * 4; ++i) {
                buff.push_back(0);
            }
        }


        for (unsigned int i = 0; i < width*height; ++i) {
            buff[i*4+3] = bytes[i];
        }


        data = buff.data();
    }

    std::stringstream output_path;
    output_path << filepath << ".png";

    stbi_write_png(output_path.str().c_str(), width, height, 4, data, width*4);

    std::stringstream msg;
    msg << "Width: " << width << " Height: " << height;
    info(msg.str().c_str());
    info("Succesfully generated file");
    
    }
    else {

        int width, height, channels;
        unsigned char* img = stbi_load(filepath, &width, &height, &channels, 0);
        if (img == NULL) {
                error("Error in loading the image\n");
                return -1;
        }


        if ((flags & OUT_SHOW) != 0) {
        
            info("File Contents: ");
            
            if ((flags & ON_ALPHA) == 0) {
                std::cout << img << std::endl;
            }
            else {
                for (unsigned i = 0; i < width * height * 4; i += 1) {
                    char ch = *(img + i);
                    std::cout << ch;
                }
                std::cout << std::endl;
            }
        }

        if ((flags & OUT_WRITE) != 0) {
                
            std::stringstream out_path;
            out_path << filepath;
                
            std::string temp = out_path.str().substr(0, out_path.str().size()-4);

            out_path.str(temp);
            
            std::ofstream write(out_path.str().c_str(), std::ios::binary);

            if (!write.is_open()) {
                std::stringstream reason;
                reason << "Couldn't open file " << out_path.str();
                error(reason.str().c_str());
                return -1;
            }

            if ((flags & ON_ALPHA) == 0) {
                for (unsigned i = 0; i < width * height * 4; i += 1) {
                    char ch = *(img + i);
                    write << ch;
                }
            }
            else {
                for (unsigned i = 3; i < width * height * 4; i += 4) {
                    char ch = *(img + i);
                    write << ch;
                }
            }

            write.close();

            info("Succesfully decoded and saved file");

        }


    }

}
