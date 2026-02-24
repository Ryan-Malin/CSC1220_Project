#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

class TraceParser
{
public:
    // Simple structure to hold one memory access
    struct Access
    {
        char operation; // 'R' or 'W'
        int pageNum;    // Which page number
    };

    // Parse a trace file and return all the accesses
    vector<Access> parseFile(const string &filename)
    {
        vector<Access> accesses;
        ifstream file(filename);

        // Check if file opened successfully
        if (!file.is_open())
        {
            cout << "Error: Could not open file " << filename << endl;
            return accesses; // Return empty vector
        }

        char op;
        int page;

        // Read line by line: "R 5", "W 10", etc.
        while (file >> op >> page)
        {
            Access access;
            access.operation = op;
            access.pageNum = page;
            accesses.push_back(access);
        }

        file.close();

        cout << "Loaded " << accesses.size() << " memory accesses from " << filename << endl;

        return accesses;
    }
};

#endif