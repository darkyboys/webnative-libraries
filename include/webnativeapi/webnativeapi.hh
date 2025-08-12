/*
 * Copyright (c) ghgltggamer 2025
 * Writeen by ghgltggamer
 * Licensed under the MIT License
 * Check out the README.md file for more information
*/

// Header
// STL
#include <filesystem>
#include   <iostream>
#include    <fstream>
#include     <string>
#include     <thread>
#include     <vector>

// Local Headers
#include <casci/casci.hh>
#include <linuxtools/linuxtools.hh>
#include <htmlui/HTMLUI.h>
#include <libbase64/libbase64.hh>

std::string wnApi_Fs_Open = "wnx0000";
std::string wnApi_DataDir = "com.global.wn";

void webnative_Api (HTMLUI& Instance, std::vector <std::string> argument_vector){
    Instance.executeJS ("wn_home_directory = `" + linuxtools::get_home_directory() + "`;");
    for (int i = 0;i < argument_vector.size();i++)
        Instance.executeJS ("wn_cli_argumentList.push(`" + CASCI(argument_vector[i]).encrypt("0") + "`);");


    Instance.registerFunction("wn_fs_newFile", [&Instance](std::string file_Path){
        std::ofstream wn_fs_newFile (file_Path);
        Instance.executeJS ("wn_event_signal = 0;");
    });

    Instance.registerFunction("wn_fs_newDir", [&Instance](std::string file_Path){
        std::filesystem::create_directories (file_Path);
        Instance.executeJS ("wn_event_signal = 0;");
    });

    Instance.registerFunction("wn_fs_remove", [&Instance](std::string file_Path){
        std::filesystem::remove_all (file_Path);
        Instance.executeJS ("wn_event_signal = 0;");
    });

    Instance.registerFunction("wn_fs_openFile", [&Instance](std::string file_Path){
        std::ifstream ifile (file_Path);
        if (ifile.is_open()){
            wnApi_Fs_Open = file_Path;
        }
        else {
            Instance.executeJS ("wn_error ('The file at `" + file_Path + +"` could not be opened.');");
        }
    });

    Instance.registerFunction("wn_fs_closeFile", [](std::string file_Path){
        wnApi_Fs_Open = "wnx0000";
    });

    Instance.registerFunction("wn_fs_writeFile", [&Instance](std::string file_Data){
        if (wnApi_Fs_Open == "wnx0000"){
            Instance.executeJS ("wn_error ('No file was open which wn_fs_writeFile can use to operate.');");
        }
        else {
            std::ofstream wn_fs_writeFile (wnApi_Fs_Open);
            wn_fs_writeFile<<file_Data;
            Instance.executeJS ("wn_event_signal = 0;");
        }
    });



    Instance.registerFunction("wn_fs_readFile", [&Instance](std::string file_Data){
        if (wnApi_Fs_Open == "wnx0000"){
            Instance.executeJS ("wn_error ('No file was open which wn_fs_readFile can use to operate.');");
        }
        else {
            std::ifstream wn_fs_readFile (wnApi_Fs_Open);
            if (wn_fs_readFile.is_open()){
                std::string buffer , content;
                while (std::getline (wn_fs_readFile, buffer)) content += buffer + '\n';
                std::string buildable_content = CASCI(content).encrypt ("0");
                Instance.executeJS ("wn_fs_readFileBuffer = `" + buildable_content + "`;");
                Instance.executeJS ("wn_event_signal = 0;");
            }
            else {
                Instance.executeJS ("wn_error ('No file was open which wn_fs_readFile can use to operate.');");
            }
        }
    });


    Instance.registerFunction("wn_fs_readFileBase64", [&Instance](std::string file_Data){
        if (wnApi_Fs_Open == "wnx0000"){
            Instance.executeJS ("wn_error ('No file was open which wn_fs_readFile can use to operate.');");
        }
        else {
            std::ifstream wn_fs_readFile (wnApi_Fs_Open);
            if (wn_fs_readFile.is_open()){
                Instance.executeJS ("wn_fs_readFileBuffer = `" + libbase64::file_to_base64(wnApi_Fs_Open) + "`;");
                Instance.executeJS ("wn_event_signal = 0;");
            }
            else {
                Instance.executeJS ("wn_error ('No file was open which wn_fs_readFile can use to operate.');");
            }
        }
    });


    Instance.registerFunction("wn_fs_isFileExists", [&Instance](std::string file_Name){
        std::ifstream wn_fs_isFileExists (file_Name);
        if (wn_fs_isFileExists.is_open()){
            Instance.executeJS ("wn_fs_isFileExistsBuffer = 1;");
        }
        else {
            Instance.executeJS ("wn_fs_isFileExistsBuffer = 0;");
        }
        Instance.executeJS ("wn_event_signal = 0;");
    });


    Instance.registerFunction("wn_fs_isDirExists", [&Instance](std::string dir_Name){
        if (std::filesystem::is_directory(dir_Name)){
            Instance.executeJS ("wn_fs_isDirExistsBuffer = 1;");
        }
        else {
            Instance.executeJS ("wn_fs_isDirExistsBuffer = 0;");
        }
        Instance.executeJS ("wn_event_signal = 0;");
    });

    // add the system calls
    Instance.registerFunction("wn_exit", [](std::string code){
        if (code [0] >= '0' and code[0] <= '9'){
            std::exit (code[0] - '0');
        }
    });

    Instance.registerFunction("wn_system", [](std::string command){
        std::system (command.c_str());
    });

    Instance.registerFunction("wn_psystem", [](std::string command){
        std::thread ([&command](){
            std::system (command.c_str());
        }).detach();
    });


    Instance.registerFunction("wn_system_piped", [&Instance](std::string command){
        std::string path = linuxtools::get_home_directory() + "/.config/" + wnApi_DataDir;
        if (std::filesystem::is_directory(path)){
            command += " >" + path + "/wn_system_piped.txt";
            // std::cout << command << "\n";
            bool finished = false;
            std::thread ([command, &finished](){
                std::cout << command.c_str() << "\n";
                std::system (command.c_str());
                finished = true;
            }).detach();
            while (not finished){
                std::ifstream ifile (path + "/wn_system_piped.txt");
                std::string buffer, content;
                while (std::getline (ifile, buffer)) content += buffer + "\n";
                content = CASCI (content).encrypt("0");
                Instance.executeJS ("wn_system_pipedBuffer = `" + content + "`;");
            }
        }
        else {
            std::filesystem::create_directories (path);
            command += " >" + path + "/wn_system_piped.txt";
            bool finished = false;
            std::thread ([command, &finished](){
                std::cout << command.c_str() << "\n";
                std::system (command.c_str());
                finished = true;
            }).detach();
            while (not finished){
                std::ifstream ifile (path + "/wn_system_piped.txt");
                std::string buffer, content;
                while (std::getline (ifile, buffer)) content += buffer + "\n";
                content = CASCI (content).encrypt("0");
                Instance.executeJS ("wn_system_pipedBuffer = `" + content + "`;");
            }
        }
        Instance.executeJS ("wn_event_signal = 0;");
    });

    Instance.registerFunction("wn_set_dataDir", [](std::string file_Path){
        wnApi_DataDir = file_Path;
    });


    Instance.executeJS ("wn_main();"); // call the wn_main to run the app
}