#include "TraceParser.h"
#include <iostream>
using namespace std;

int main()
{
    TraceParser parser;

    // Parse the trace file
    vector<TraceParser::Access> accesses = parser.parseFile("test_trace.txt");

    // Print what we got
    cout << "\n=== Parsed Accesses ===" << endl;
    for (int i = 0; i < accesses.size(); i++)
    {
        cout << i << ": " << accesses[i].operation << " " << accesses[i].pageNum << endl;
    }

    return 0;
}