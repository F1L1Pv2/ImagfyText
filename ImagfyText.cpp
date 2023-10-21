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
#include <string>


char* shift_args(int& argc, char**& argv) {
    if (argc > 0) {
        char* arg = *argv;
        argc -= 1;
        argv += 1;
        return arg;
    }
    return nullptr;
}

void error(const char* reason) {
    std::cout << "[Error]: " << reason << std::endl;
}

void usage(const char* program, const char* reason = nullptr) {
    if (reason != nullptr) error(reason);
    std::cout << "[Usage]: Modes: \"encode\", \"decode\"" << std::endl;
    std::cout << "[Usage]:        \"encode\" + <filepath> + <image_width>" << std::endl;
    //std::cout << "[Note]:         file needs to be divisable by 4 and <image_width>" << std::endl;
    std::cout << "[Usage]:        \"decode\" + <filepath>" << std::endl;
    std::cout << "[Usage]: Decode flags: \"binary\", \"out_write\"" << std::endl;
    std::cout << "[Usage]:               \"binary\" - file will be interpreted as bytecode rather than ascii characters (file wont be printed but it will be saved on drive)" << std::endl;
    std::cout << "[Usage]:               \"out_write\" - file output will be saved into file" << std::endl;
    std::cout << "[Note]:         <filepath> needs to exist" << std::endl;
    std::cout << "[Usage]: " << program << " <mode> <additional params>" << std::endl;
}

const unsigned int BINARY_MODE = 1;  //01
const unsigned int OUT_WRITE = 2;    //10

int main(int argc, char** argv)
{

    unsigned int flags = 0;

    const char* program = shift_args(argc, argv);

    if (argc <= 0) {
        usage(program, "mode wasn't provided");
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

    const char* str_width = nullptr;

    if (mode_str == "encode") {
        if (argc <= 0) {
            usage(program, "image width wasn't provided");
            return -1;
        }

        str_width = shift_args(argc, argv);
    }

    if (mode_str == "decode") {
        while (argc > 0) {
            char* flag_chr = shift_args(argc, argv);
            std::string flag;
            flag = flag_chr;

            if (flag == "binary") {
            
                flags |= BINARY_MODE;
                flags |= OUT_WRITE;
            
            }
            else if (flag == "out_write") {
                flags |= OUT_WRITE;
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
        usage("File doesn't exist");
        return -1;
    }

    std::vector<char> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));

    input.close();

    while (bytes.size() % 4 != 0) {
        bytes.push_back(0);
        //error("input file has byte count not dividable by 4");
        //return -1;
    }

    unsigned int pixels_count = bytes.size() / 4;

    std::stringstream strValue;
    strValue << str_width;

    unsigned int width;
    strValue >> width;

    while (pixels_count % width != 0) {
        
        bytes.push_back(0);

        pixels_count = bytes.size() / 4;
        
        //error("input file byte count isnt dividable by desired width");
        //return -1;
    }

    unsigned int height = pixels_count / width;

    std::stringstream output_path;
    output_path << filepath << ".png";

    stbi_write_png(output_path.str().c_str(), width, height, 4, bytes.data(), width * 4);

    std::cout << "Succesfully generated file" << std::endl;
    
    }
    else {

        int width, height, channels;
        unsigned char* img = stbi_load(filepath, &width, &height, &channels, 0);
        if (img == NULL) {
                printf("Error in loading the image\n");
                exit(1);
        }

        if ((flags & BINARY_MODE) == 0) {
            std::cout << img << std::endl;
        }

        if ((flags & OUT_WRITE) != 0) {
            //#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
                
            std::stringstream out_path;
            out_path << filepath;

            if ((flags & BINARY_MODE) != 0) {
                out_path << ".lol";
            }
            else {
                out_path << ".txt";
            }
            
            std::ofstream write(out_path.str().c_str(), std::ios::binary);

            for (unsigned i = 0; i < width * height * 4; i += 1) {
                char ch = *(img + i);
                write << ch;
            }

            write.close();

            std::cout << "Succesfully decoded and saved file" << std::endl;


            //#else
            //static_assert("linux isn't supported yet");
            //#endif
        }


    }

}
