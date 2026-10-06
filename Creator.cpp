#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "employee.h"

int main(int argc, char* argv[])
{
    // argv[0] = program name, argv[1] = file name, argv[2] = records count
    if (argc != 3)
    {
        std::cerr << "Usage: Creator <binary file> <records count>\n";
        return 1;
    }

    const char* fileName = argv[1];
    int count = std::atoi(argv[2]);
    if (count <= 0)
    {
        std::cerr << "Records count must be a positive number\n";
        return 1;
    }

    std::ofstream fout(fileName, std::ios::binary);
    if (!fout)
    {
        std::cerr << "Cannot create file " << fileName << '\n';
        return 1;
    }

    for (int i = 0; i < count; ++i)
    {
        employee emp = {};   // zero-fill the record

        std::cout << "Employee #" << i + 1 << '\n';

        std::cout << "  id: ";
        std::cin >> emp.num;

        std::cout << "  name (up to 9 letters): ";
        std::string name;
        std::cin >> name;                                // the whole word, any length
        name.copy(emp.name, sizeof(emp.name) - 1);       // copy at most 9 chars; emp.name[9] stays '\0'

        std::cout << "  hours: ";
        std::cin >> emp.hours;

        fout.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    return 0;   // fout is closed automatically here
}
