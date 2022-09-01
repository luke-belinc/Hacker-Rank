#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   

    fstream in("sample_in.txt", fstream::in);

    if (!in.is_open()) {
        cout << "\ncould not open file...\n";
        return -1;
    }

    // store number of ml lines and query lines
    int nLines;
    int nQueries;

    // read numbers from first line
    in >> nLines >> nQueries;

    // vector for storing queries and ml
    vector<string> lines;
    vector<string> queries;

    // string for storing output from getline
    string line;
    // clear empty line from reading numbers
    getline(in, line);

    // get markup language lines
    for(int i = 0; i < nLines; i++) {
        getline(in, line);
        lines.push_back(line);
    }
    // get query lines
    for(int i = 0; i < nQueries; i++) {
        getline(in, line);
        queries.push_back(line);
    }

    // print out values of lines
    for (string s : lines) {
        cout << s << endl;
    }
    // print out values of queries
    for (string s : queries) {
        cout << s << endl;
    }

    in.close();

    return 0;
}
