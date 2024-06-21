#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

string dectobin(int decimal) {
    string s = "";
    for (int i = 7; i >= 0; i--) {
        if (decimal >> i & 1) s += '1';
        else s += '0';
    }
    return s;
}

void writeFileType(ifstream &input, string &outputFileName) {
    char buff[1];
    input.read(buff, 1);
    int ftsize = buff[0] - '0';
    while (ftsize--) {
        input.read(buff, 1);
        outputFileName += buff[0];
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Invalid cmd line arg. Usage: ./a.out <input file>\n";
        return 1;
    }

    ifstream input(argv[1], ios::binary);
    if (!input.is_open()) {
        cout << "Error opening input file\n";
        return 2;
    }

    string inputFileName = argv[1];
    string outputFileName = inputFileName.substr(0, inputFileName.find("-compressed")) + "-decompressed.";
    writeFileType(input, outputFileName);

    ofstream output(outputFileName, ios::binary);
    if (!output.is_open()) {
        cout << "Error creating output file\n";
        return 3;
    }

    // Reading the codes from the file
    vector<int> compressedData;
    char buff[1];
    while (input.read(buff, 1)) {
        compressedData.push_back((unsigned char)buff[0]);
    }

    // LZW decompression logic
    int dictSize = 256;
    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, i);
    }

    int bitsPerCode = 12;
    int buffer = 0, bufferBits = 0;
    vector<int> codes;

    for (char byte : compressedData) {
        buffer = (buffer << 8) | (unsigned char)byte;
        bufferBits += 8;

        while (bufferBits >= bitsPerCode) {
            int code = (buffer >> (bufferBits - bitsPerCode)) & ((1 << bitsPerCode) - 1);
            codes.push_back(code);
            bufferBits -= bitsPerCode;
        }
    }

    string result;
    string w(1, codes[0]);
    result += w;

    for (size_t i = 1; i < codes.size(); ++i) {
        int k = codes[i];
        string entry;
        if (dictionary.count(k)) {
            entry = dictionary[k];
        } else if (k == dictSize) {
            entry = w + w[0];
        } else {
            cerr << "Error in decompression\n";
            return 4;
        }
        result += entry;

        dictionary[dictSize++] = w + entry[0];
        w = entry;
    }

    output.write(result.c_str(), result.size());

    cout << "Decompressed File Successfully.\n";

    input.close();
    output.close();
    return 0;
}
