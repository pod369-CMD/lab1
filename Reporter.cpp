#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "employee.h"

int main(int argc, char* argv[])
{
    // argv[1] = binary file, argv[2] = report file, argv[3] = pay per hour
    if (argc != 4)
    {
        std::cerr << "Usage: Reporter <binary file> <report file> <pay per hour>\n";
        return 1;
    }

    const char* binName    = argv[1];
    const char* reportName = argv[2];
    double rate = std::atof(argv[3]);

    // 1. Read all records from the binary file
    std::ifstream fin(binName, std::ios::binary);
    if (!fin)
    {
        std::cerr << "Cannot open file " << binName << '\n';
        return 1;
    }

    std::vector<employee> list;
    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
        list.push_back(emp);
    fin.close();

    // 2. Sort by employee id (ascending)
    std::sort(list.begin(), list.end(),
              [](const employee& a, const employee& b) { return a.num < b.num; });

    // 3. Write the text report
    std::ofstream fout(reportName);
    if (!fout)
    {
        std::cerr << "Cannot create file " << reportName << '\n';
        return 1;
    }

    fout << "Report on file \"" << binName << "\"\n";
    fout << "ID, name, hours, salary\n";
    fout << std::fixed << std::setprecision(2);
    for (const employee& e : list)
        fout << e.num << ", " << e.name << ", " << e.hours << ", " << e.hours * rate << '\n';

    return 0;
}
