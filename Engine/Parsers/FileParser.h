#include <fstream>
#include <string>

/**
 * Provides functions for reading and writing files.
 */
namespace FileParser
{
    /**
     * Reads the contents of a file specified by the given file path.
     *
     * @param file_path The path to the file to be read.
     * @return A string containing the entire contents of the file.
     *         Returns an empty string if the file cannot be opened.
     */
    std::string Read(const std::string &file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
            return "";

        std::string json_string;
        std::string line;
        while (std::getline(file, line))
            json_string += line;

        file.close();

        return json_string;
    }

    /**
     * Writes a string to a specified file.
     *
     * Opens the file at the given file path and writes the provided string to it.
     * If the file cannot be opened, the function returns without performing any action.
     *
     * @param file_path The path to the file where the string will be written.
     * @param str The string content to be written to the file.
     */
    void Write(const std::string &file_path, const std::string &str)
    {
        std::ofstream file(file_path);
        if (!file.is_open())
            return;

        file << str;
        file.close();
    }
}