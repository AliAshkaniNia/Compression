#ifndef __IFILE_HANDLER_H__
#define __IFILE_HANDLER_H__
#include <string_view>
#include <string>

namespace FileHandlers
{
    class IFileHandler
    {
        public:
        virtual ~IFileHandler() = default;

        /**
         * @brief
         * I chose to use init function instead of constructor.
         * as it is not common to include a constructor in an interface.
         * Constructors are generally considered to be implementation details.
         */
        virtual void init(std::string_view input_file_path, std::string_view output_file_path) = 0;
        virtual int load(std::string &content) = 0;
        virtual int save(std::string_view content) = 0;
    };
};

#endif