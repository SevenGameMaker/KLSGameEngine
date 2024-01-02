// include the needed header files
#include "KLS_File.h"
#include "KLS_Logger.h"
#include "KLS_String.h"
#include <fstream>
#include <iosfwd>
#include <direct.h>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

// unclutter the global namespace
namespace KLS
{
    // copy the delay loaded dll files into the working dirctory
    bool setupKLSSystems()
    {
#ifdef KLS_DELAYLOAD
        //       // delete all of th DLL files that are in the current directory
        //      if (!deleteFiles("deleting old DLL files", FILES_TO_DELETE))
        //       {
        //          KLS_ERROR("Error : unable to delete previous DLL files - aborting program");
        //           return false;
        //      }

               // based upon the build, copy the required (delay loaded) DLL files
        fs::path currentDir = fs::current_path();
        std::string cd = std::string(currentDir.string());
        cd += "\\";
        if (!copyFiles(cd + std::string(FILE_COPY_SOURCE), cd))
        {
            KLS_ERROR("Error : unable to copy correct DLL files - aborting program");
            return false;
        }
#endif

        if (!fs::copy_file(ComponentSourcePathStr, ComponentDestPathStr, fs::copy_options::overwrite_existing))
        {
            KLS_ERROR("unable to copy component definition file");
            return KLS_FAILURE;
        }

        // everything went fine
        return KLS_SUCCESS;
    }

    // delete the delayed dll files from tthe working directory
    bool deleteFiles(std::string text, std::string listFilename)
    {
        // always print, even in release mode
        KLS_WARN("  deleting files {} {}", text.c_str(), listFilename.c_str());

        std::ifstream file(listFilename);
        if (!file.is_open())
        {
            KLS_ERROR("Could not open file: {}", listFilename.c_str());
            return KLS_FAILURE;
        }

        std::vector<std::string> filenames;
        std::string line;
        while (std::getline(file, line))
        {
            filenames.push_back(line);
        }

        for (const std::string& filename : filenames)
        {
            KLS_INFO("  Deleting file {}", filename.c_str());
            if (!remove(filename.c_str()))
            {
                KLS_ERROR("  Failed to delete: {}", filename.c_str());
                file.close();
                return KLS_FAILURE;
            }
        }

        file.close();

        // everything went fine
        return true;
    }

    // copies files from the sourcedir directory into the destdir
    bool copyFiles(std::string sourceDir, std::string destDir)
    {
        KLS_WARN("copying files from {}", sourceDir.c_str());

        fs::path currentDir = fs::current_path();

        for (const auto& entry : fs::directory_iterator(sourceDir))
        {
            if (fs::is_regular_file(entry))
            {
                const std::string& sourcePathStr = entry.path().string();
                const std::string& destPathStr = (fs::path(destDir) / entry.path().filename()).string();
                if (fs::copy_file(sourcePathStr, destPathStr, fs::copy_options::overwrite_existing))
                {
                    KLS_INFO("Copied: {} ", sourcePathStr.c_str());
                }
                else
                {
                    KLS_ERROR(" unable to copy {} ", sourcePathStr.c_str());
                    return KLS_FAILURE;
                }
            }
        }


        fs::current_path(currentDir);
        KLS_WARN("  changed back to directory {}", currentDir.string().c_str());

        return KLS_SUCCESS;
    }


    std::string readFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file)
        {
            KLS_WARN("readfile : unable to open file {}", filename.c_str());
            return std::string();
        }

        file.ignore(std::numeric_limits<std::streamsize>::max());
        auto size = file.gcount();

        if (size > 0x10000) // 64KiB sanity check for shaders:
        {
            KLS_WARN("this file is huge! {}", filename.c_str());
            return std::string();
        }

        file.clear();
        file.seekg(0, std::ios_base::beg);

        std::stringstream sstr;
        sstr << file.rdbuf();
        file.close();
        return std::string(sstr.str());
    }

    char* ReadBinaryFile(const char* pFilename, int& size)
    {
        FILE* f = NULL;

        errno_t err = fopen_s(&f, pFilename, "rb");

        if (!f) {
            char buf[256] = { 0 };
            strerror_s(buf, sizeof(buf), err);
            KLS_ERROR("Error opening '%s': %s\n", pFilename, buf);
            exit(0);
        }

        struct stat stat_buf;
        int error = stat(pFilename, &stat_buf);

        if (error) {
            char buf[256] = { 0 };
            strerror_s(buf, sizeof(buf), err);
            KLS_ERROR("Error getting file stats: %s\n", buf);
            return NULL;
        }

        size = stat_buf.st_size;

        char* p = (char*)malloc(size);
        assert(p);

        size_t bytes_read = fread(p, 1, size, f);

        if (bytes_read != size) {
            char buf[256] = { 0 };
            strerror_s(buf, sizeof(buf), err);
            KLS_ERROR("Read file error file: %s\n", buf);
            exit(0);
        }

        fclose(f);

        return p;
    }

    std::string extractFilePath(const std::string& filename)
    {
        size_t lastSlashPos = filename.find_last_of("/\\");
        if (lastSlashPos != std::string::npos)
        {
            return filename.substr(0, lastSlashPos + 1);
        }
        return "";
    }

    std::vector<std::string> listFilesWithExtension(const std::string& directoryPath, const std::string& fileExtension) 
    {
        std::vector<std::string> fileNames;

             for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                if (entry.is_regular_file()) {
                    std::string currentFileExtension = entry.path().extension().string();
                    if (currentFileExtension == fileExtension) {
                        fileNames.push_back(entry.path().filename().string());
                    }
                }
            }
 
        return fileNames;
    }

    std::string KLS_API addFolderToFilename(std::string filename, std::string newfolder)
    {
        // Find the last occurrence of the directory separator
        size_t lastSlashPos = filename.find_last_of('/');
        if (lastSlashPos == std::string::npos) {
            // If not found, consider the last occurrence of '\'
            lastSlashPos = filename.find_last_of('\\');
        }

        if (lastSlashPos != std::string::npos) {
            // Insert the new folder name and a directory separator after the last directory
            return filename.substr(0, lastSlashPos + 1) + newfolder + '/' + filename.substr(lastSlashPos + 1);
        }
        return filename;
    }

    std::string addTextToFilename(std::string filename, std::string text)
    {
        size_t dotPosition = filename.find_last_of('.');
        if (dotPosition != std::string::npos) 
        {
            filename.insert(dotPosition, text);
        }
        return filename;
    }

    std::string getFile(std::string filename, std::string startdir, std::string filter)
    {
        OPENFILENAMEW ofn;
        wchar_t szFileName[MAX_PATH] = L"";
        ZeroMemory(&ofn, sizeof(ofn));

        std::wstring f = stringToWideString(filter);
        std::wstring d = stringToWideString(startdir);

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = f.c_str(); // L"All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
        ofn.lpstrInitialDir = d.c_str(); 

        if (GetOpenFileNameW(&ofn))
        {
            return widestringToString(szFileName);
        }
        return filename;
    }
} // end namespace

