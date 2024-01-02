// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"

// unclutter the global namespace
namespace KLS
{
    // copy the delay loaded dll files into the working dirctory
    extern bool setupKLSSystems();

    // deletes the files (listed in the listFilename document) from the the working directory
    extern bool deleteFiles(std::string text, std::string listFilename);

    // copies files from the _Files directory into the working directory
    extern bool copyFiles(std::string sourceDir, std::string destDir);

    // read a text file, returning a string that holds the file contents
    extern std::string readFile(const std::string& filename);

    // read a binary file, returning a char* that holds the file contents
    extern char* ReadBinaryFile(const char* pFileName, int& size);

    // get the filepath from the filename (example "../../../../_media/_textures/default.png" returns "../../../../_media/_textures/" 
    extern std::string extractFilePath(const std::string& filename);

    // get a list of files (by extension) in a directory
    extern std::vector<std::string> KLS_API listFilesWithExtension(const std::string& directoryPath, const std::string& fileExtension);

    extern std::string KLS_API addFolderToFilename(std::string filename, std::string newfolder);
    extern std::string KLS_API addTextToFilename(std::string filename, std::string text);

    extern std::string KLS_API getFile(std::string filename, std::string startdir, std::string filter);
} // end namespace
