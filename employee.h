#pragma once

// One record of the binary file.
struct employee
{
    int    num;       // employee id
    char   name[10];  // employee name (up to 9 letters + terminating '\0')
    double hours;     // hours worked
};
