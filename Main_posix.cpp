#include <unistd.h>      // fork, execv, _exit
#include <sys/wait.h>    // waitpid, WIFEXITED, WEXITSTATUS
#include <cstdio>        // perror
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "employee.h"

// Starts a program, waits until it finishes.
// Returns true only if the program started and finished with exit code 0.
bool runAndWait(const std::string& program, const std::vector<std::string>& args)
{
    pid_t pid = fork();              // clone the current process
    if (pid < 0)
    {
        perror("fork");
        return false;
    }

    if (pid == 0)                    // ---- this code runs in the CHILD ----
    {
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(program.c_str()));      // argv[0] = program name
        for (const std::string& a : args)
            argv.push_back(const_cast<char*>(a.c_str()));        // argv[1], argv[2], ...
        argv.push_back(nullptr);                                 // the list must end with nullptr

        execv(program.c_str(), argv.data());   // replace the child with the new program
        perror("execv");                       // we get here only if execv failed
        _exit(1);
    }

    int status = 0;                  // ---- this code runs in the PARENT ----
    waitpid(pid, &status, 0);        // wait for the child to finish
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

void printBinaryFile(const std::string& name)
{
    std::ifstream fin(name, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Cannot open file " << name << '\n';
        return;
    }

    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
        std::cout << emp.num << ' ' << emp.name << ' ' << emp.hours << '\n';
}

void printTextFile(const std::string& name)
{
    std::ifstream fin(name);
    if (!fin)
    {
        std::cerr << "Cannot open file " << name << '\n';
        return;
    }

    std::string line;
    while (std::getline(fin, line))
        std::cout << line << '\n';
}

int main()
{
    std::string binName, reportName;
    int count;
    double rate;

    std::cout << "Binary file name: ";
    std::cin >> binName;
    std::cout << "Number of records: ";
    std::cin >> count;

    if (!runAndWait("./Creator", { binName, std::to_string(count) }))
        return 1;

    std::cout << "\nContents of " << binName << ":\n";
    printBinaryFile(binName);

    std::cout << "\nReport file name: ";
    std::cin >> reportName;
    std::cout << "Pay per hour: ";
    std::cin >> rate;

    if (!runAndWait("./Reporter", { binName, reportName, std::to_string(rate) }))
        return 1;

    std::cout << "\nReport:\n";
    printTextFile(reportName);

    return 0;
}
