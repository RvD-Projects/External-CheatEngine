#include <iostream>
#include <string>
#include <stdexcept>
#include <array>
#include <vector>
#include <sstream>
#include <cstdio>

/**
 * @class ShellCommand
 *
 * @brief Provides functionality to execute shell commands and retrieve their output.
 *
 * This class allows the execution of shell commands with optional arguments and
 * captures the output of the command. It uses a static method to run the command
 * and another to retrieve the result. The command execution is handled internally
 * using the popen function to open a pipe to the shell.
 *
 * @note The class is designed to handle command execution in a synchronous manner.
 */
class ShellCommand
{
public:
    /**
     * Executes a shell command with optional arguments.
     *
     * Constructs a command string by appending the provided arguments to the base command,
     * then executes the command in a shell environment. The full command string is stored
     * in the static member `fullCmd` and the result of the execution is returned.
     *
     * @param cmd The base shell command to execute.
     * @param args Optional vector of arguments to append to the command.
     * @return The result of the executed command as a string.
     * @throws std::runtime_error if the command execution fails.
     */
    static std::string Run(const std::string &cmd, const std::vector<std::string> &args = {})
    {
        std::stringstream cmdWithArgs;
        cmdWithArgs << cmd;

        for (const auto &arg : args)
            cmdWithArgs << " " << arg;

        fullCmd = cmdWithArgs.str();

        return Execute();
    }

    static std::string GetResult()
    {
        return result;
    }

private:
    static std::string result;
    static std::string fullCmd;

    /**
     * Executes the stored shell command and captures its output.
     *
     * Opens a pipe to execute the command stored in `fullCmd` using `popen`,
     * reads the command's output into a buffer, and appends it to `result`.
     * Returns the complete output of the executed command.
     *
     * @return The output of the executed shell command as a string.
     * @throws std::runtime_error if the pipe cannot be opened.
     */
    static std::string Execute()
    {
        std::array<char, 128> buffer;
        std::shared_ptr<FILE> pipe(popen(fullCmd.c_str(), "r"), pclose);

        if (!pipe)
            throw std::runtime_error("popen() failed!");

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            result += buffer.data();

        return GetResult();
    }
};