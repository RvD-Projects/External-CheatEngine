#include <fstream>
#include "json.hpp"

/**
 * Provides functions for parsing JSON data.
 */
namespace JsonParser
{
    // https://json.nlohmann.me/
    using Json = nlohmann::json;

    /**
     * Parses a JSON file from the specified file path.
     *
     * @param file_path The path to the JSON file to be parsed.
     * @return A Json object containing the parsed data, or nullptr if the file cannot be opened.
     */
    Json ParseFile(const std::string &file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
            return nullptr;

        Json json;
        file >> json;
        file.close();

        return json;
    }

    /**
     * Writes a JSON object to a file specified by the given file path.
     *
     * @param file_path The path to the file where the JSON object will be written.
     * @param json The JSON object to be written to the file.
     */
    void WriteFile(const std::string &file_path, const Json &json)
    {
        std::ofstream file(file_path);
        if (!file.is_open())
            return;

        file << json.dump(4);
        file.close();
    }

    /**
     * Parses a JSON string.
     *
     * @param json_string The JSON string to be parsed.
     * @return A Json object containing the parsed data.
     */
    Json Parse(const std::string &json_string)
    {
        return Json::parse(json_string);
    }

    /**
     * Converts a JSON object to a string.
     *
     * @param json The JSON object to be converted to a string.
     * @return A string representation of the JSON object.
     */
    std::string Stringify(const Json &json)
    {
        return json.dump(4);
    }
}