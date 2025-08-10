/*
 * Copyright (c) ghgltggamer 2025
 * Writeen by ghgltggamer
 * Licensed under the MIT License
 * Check out the README.md file for more information
*/

// Header
// STL
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

namespace libbase64 {
    // Base64 encoding table
    static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string base64_encode(const std::vector<unsigned char>& data) {
        std::string encoded;
        size_t i = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        for (size_t pos = 0; pos < data.size(); ++pos) {
            char_array_3[i++] = data[pos];
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; i < 4; i++)
                    encoded += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (size_t j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (size_t j = 0; j < i + 1; j++)
                encoded += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                encoded += '=';
        }

        return encoded;
    }

    // Function to read file and return Base64 string
    std::string file_to_base64(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (!file)
            throw std::runtime_error("Could not open file: " + filepath);

        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        return base64_encode(buffer);
    }
}