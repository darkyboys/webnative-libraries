/*
 * Copyright (c) ghgltggamer 2025
 * Written by ghgltggamer 2025
 * Licensed under the MIT License - Please check the License file for more information
 * CASCI - A Secure Format to store data & information in a secure way using patterns
 * CASCI - Stands for Characterised Alternate Secure Calendar Integral
 */

class CASCI {
    constructor(value) {
        this.data = value;
    }

    encrypt(pattern) {
        let result = '';
        let patternIndex = 0;

        const lines = this.data.split('\n');
        for (const line of lines) {
            for (let i = 0; i < line.length; i++) {
                const charCode = line.charCodeAt(i);
                const patternValue = parseInt(pattern[patternIndex], 10);
                result += (charCode + patternValue) + ' ';

                patternIndex = (patternIndex + 1) % pattern.length;
            }
            result += '\n';
        }

        return result + '\n';
    }

    decrypt(pattern) {
        let result = '';
        let patternIndex = 0;

        let current = '';
        for (let i = 0; i < this.data.length; i++) {
            const ch = this.data[i];

            if (ch === '\n') {
                result += '\n';
                continue;
            }

            if (ch === ' ') {
                if (current.length > 0) {
                    const value = parseInt(current, 10);
                    const patternValue = parseInt(pattern[patternIndex], 10);
                    result += String.fromCharCode(value - patternValue);

                    patternIndex = (patternIndex + 1) % pattern.length;
                    current = '';
                }
            } else {
                current += ch;
            }
        }

        return result;
    }
}
// lib casci end



/*
 * Copyright (c) ghgltggamer 2025
 * Writeen by ghgltggamer
 * Licensed under the MIT License
 * Check out the README.md file for more information
*/

let wn_event_signal = 0; // as there is no event
let wn_home_directory = ""; // this must be defined when ever you compile your program into wn native
let wn_cli_argumentList = []; // this will contain the argument list from the cli


function wn_error (error){
    console.error (error);
    document.body.innerHTML = error;
}

let wnApi;

function wn_init (){
    let rval = false;
    if (typeof window.nativeBridge == 'object' && typeof window.nativeBridge.invoke == 'function') {
        rval = true;
        wnApi = window.nativeBridge;
    }
    return rval;
}


function wn_run (func){
    if (wn_init()) func();
    else wn_error ("Wn Api was not initalised, Make sure to compile your JavaScript program with the WNtoolChain in order to make it working");
}

function wn_fs_newFile (file_Path){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke ('wn_fs_newFile', file_Path);
    })
}

let wn_continue_globalChecks = 0;
function wn_continue (func, checks = 100){
    let interval = setInterval (()=>{
        if (!wn_event_signal){
            func();
            clearInterval(interval);
        }
    }, (wn_continue_globalChecks == 0)? checks : wn_continue_globalChecks);
}



// Fs functions
function wn_fs_openFile(file_Path){
    wn_run (()=>{
        wnApi.invoke ('wn_fs_openFile', file_Path);
    })
}


function wn_fs_writeFile(file_Data){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke ('wn_fs_writeFile', file_Data);
    })
}

let wn_fs_readFileBuffer = "";
function wn_fs_readFile(){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke ('wn_fs_readFile');
    });
}

function wn_fs_readFileBase64(){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke ('wn_fs_readFileBase64');
    });
}

function wn_fs_closeFile(){
    wn_run(()=>{
        wnApi.invoke ('wn_fs_closeFile', "null");
    })
}

let wn_fs_isFileExistsBuffer = 0;
function wn_fs_isFileExists(file_Name){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke('wn_fs_isFileExists', file_Name);
    });
}

let wn_fs_isDirExistsBuffer = 0;
function wn_fs_isDirExists(dir_Name){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke('wn_fs_isDirExists', dir_Name);
    });
}

function wn_fs_newDir(dir_Name){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke('wn_fs_newDir', dir_Name);
    });
}

function wn_fs_remove(dir_Name){
    wn_run(()=>{
        wn_event_signal = 1;
        wnApi.invoke('wn_fs_remove', dir_Name);
    });
}

// wn core fucntions

function wn_normalize (bufferData){
    let normalizer = new CASCI(bufferData);
    return normalizer.decrypt(
        "0"
    );
}


function wn_exit (code = 3){
    if (typeof code != 'number'){
        wnApi.invoke ("wn_exit", '3');
    }
    wn_run (()=>{
        wnApi.invoke ("wn_exit", code);
    });
}


function wn_system (command){
    wn_run (()=>{
        wnApi.invoke ("wn_system", command);
    });
}

let wn_system_pipedBuffer = ``;
function wn_system_piped (command){
    wn_run (()=>{
        wn_event_signal = 0;
        wnApi.invoke ("wn_system_piped", command);
    });
}

function wn_set_dataDir (dir){
    wn_run (()=>{
        wnApi.invoke ("wn_set_dataDir", dir);
    });
}


function wn_psystem (command){
    wn_run (()=>{
        wnApi.invoke ("wn_psystem", command);
    });
}