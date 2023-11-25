#ifndef __UNIX_FILE_HANDLER_H__
#define __UNIX_FILE_HANDLER_H__

#include "iFileHandler.h"
#include <string>

namespace FileHandlers{
    class UnixFileHandler : public IFileHandler
    {
    public:
        UnixFileHandler() = default;

        void init(std::string_view input_file_path, std::string_view output_file_path) override;
        int load(std::string& content) override;
        int save(std::string_view content) override;
        ~UnixFileHandler() = default;
    private:
        std::string input_file_path, output_file_path;
    };

};

#endif 