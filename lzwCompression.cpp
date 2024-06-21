#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

#define io                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

using namespace std;

void writeFileType(ofstream &output, const string &filetype) {
    int ftsize = filetype.length();
    char num = ftsize + '0';
    output.write(&num, 1);
    output.write(filetype.c_str(), ftsize);
}

void compressLZW(const string &inputFileName, const string &outputFileName) {
    ifstream input(inputFileName, ios::binary);
    if (!input.is_open()) {
        cerr << "Error opening input file\n";
        exit(2);
    }

    string filetype = inputFileName.substr(inputFileName.find(".") + 1);
    ofstream output(outputFileName, ios::binary);
    if (!output.is_open()) {
        cerr << "Error creating output file\n";
        exit(3);
    }

    writeFileType(output, filetype);

    unordered_map<string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[string(1, i)] = i;
    }

    string s;
    char c;
    vector<int> compressedData;
    int dictSize = 256;

    while (input.get(c)) {
        string sc = s + c;
        if (dictionary.count(sc)) {
            s = sc;
        } else {
            compressedData.push_back(dictionary[s]);
            if (dictSize < 4096) { // LZW standard limit for 12-bit codes
                dictionary[sc] = dictSize++;
            }
            s = string(1, c);
        }
    }

    if (!s.empty()) {
        compressedData.push_back(dictionary[s]);
    }

    int bitsPerCode = 12; // Assuming 12-bit codes for the LZW algorithm
    int buffer = 0, bufferBits = 0;

    for (int code : compressedData) {
        buffer |= code << (32 - bitsPerCode - bufferBits);
        bufferBits += bitsPerCode;

        while (bufferBits >= 8) {
            char byte = buffer >> 24;
            output.write(&byte, 1);
            buffer <<= 8;
            bufferBits -= 8;
        }
    }

    if (bufferBits > 0) {
        char byte = buffer >> 24;
        output.write(&byte, 1);
    }

    input.close();
    output.close();
}

int main(int argc, char *argv[]) {
    io;
    if (argc != 2) {
        cout << "Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return 1;
    }

    string inputFileName = argv[1];
    string outputFileName = inputFileName.substr(0, inputFileName.find(".")) + "-compressed.bin";

    compressLZW(inputFileName, outputFileName);
    cout << "Compressed file successfully\n";

    return 0;
}
