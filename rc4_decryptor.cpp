#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// RC4 Key-Scheduling Algorithm (KSA)
void rc4KSA(vector<uint8_t>& S, const string& key) {
    S.resize(256);

    for (int i = 0; i < 256; ++i) {
        S[i] = static_cast<uint8_t>(i);
    }

    int j = 0;

    for (int i = 0; i < 256; ++i) {
        j = (j + S[i] + static_cast<uint8_t>(key[i % key.length()])) % 256;
        swap(S[i], S[j]);
    }
}

// RC4 Pseudo-Random Generation Algorithm (PRGA)
void rc4Crypt(vector<uint8_t>& data, const string& key) {
    vector<uint8_t> S;
    rc4KSA(S, key);

    int i = 0;
    int j = 0;

    for (size_t k = 0; k < data.size(); ++k) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        swap(S[i], S[j]);

        int t = (S[i] + S[j]) % 256;
        uint8_t keyStreamByte = S[t];

        data[k] ^= keyStreamByte;
    }
}

int main() {
    const string filename = "file.txt";
    const string key = "MySecretKey123";

    // Open encrypted file in binary mode
    ifstream inputFile(filename, ios::binary);

    if (!inputFile) {
        cerr << "Error: Could not open " << filename << endl;
        return 1;
    }

    // Read encrypted contents into memory
    vector<uint8_t> data(
        (istreambuf_iterator<char>(inputFile)),
        istreambuf_iterator<char>()
    );

    inputFile.close();

    cout << "Read " << data.size()
         << " encrypted bytes from " << filename << endl;

    // Decrypt using the same RC4 key
    rc4Crypt(data, key);

    // Write decrypted data back to file.txt
    ofstream outputFile(filename, ios::binary | ios::trunc);

    if (!outputFile) {
        cerr << "Error: Could not write to " << filename << endl;
        return 1;
    }

    outputFile.write(
        reinterpret_cast<const char*>(data.data()),
        data.size()
    );

    outputFile.close();

    cout << "File decrypted successfully using RC4." << endl;

    return 0;
}