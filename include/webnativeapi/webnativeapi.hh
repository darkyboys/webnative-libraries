/*
 * Copyright (c) ghgltggamer 2025
 * Writeen by ghgltggamer
 * Licensed under the MIT License
 * Check out the README.md file for more information
*/

// Header
// STL
#include <iostream>
#include  <fstream>
#include   <string>
#include   <thread>
#include   <vector>

// Local Headers
#include <casci/casci.hh>
#include <linuxtools/linuxtools.hh>
#include <htmlui/HTMLUI.h>

std::string wnApi_Fs_Open = "wnx0000";

void webnative_Api (HTMLUI& Instance, std::vector <std::string> argument_vector){
    Instance.executeJS ("wn_home_directory = `" + linuxtools::get_home_directory() + "`;");
    for (int i = 0;i < argument_vector.size();i++)
        Instance.executeJS ("wn_cli_argumentList.push(`" + CASCI(argument_vector[i]).encrypt("0") + "`);");
    

    Instance.registerFunction("wn_fs_newFile", [](std::string file_Path){
        std::ofstream wn_fs_newFile (file_Path);
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
                Instance.executeJS ("wn_fs_readFileData = `" + buildable_content + "`;");
                Instance.executeJS ("wn_event_signal = 0;");
            }
            else {
                Instance.executeJS ("wn_error ('No file was open which wn_fs_readFile can use to operate.');");
            }
        }
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


    Instance.executeJS ("wn_main();"); // call the wn_main to run the app
}