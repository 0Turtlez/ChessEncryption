#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <bitset>
#include <chrono>

using namespace std::chrono;
using namespace std;

// P - pawn, N - knight, B - bishop, K - King, R - Rook, Q - queen
// Upper case, white, Lower case, black
const char emptySquare = ' ';
const int BASE = 13;
const int BITS_PER_CHAR = 5;
const int TOTAL_SQUARES = 64;

// 32-symbol custom charset
const string CHARSET = " .,;?!ABCDEFGHIJKLMNOPQRSTUVWXYZ";
map<char, int> charToCode;
map<int, char> codeToChar;

// 13-symbol pieceset
const string PIECESET = "1PNBKRQpnbkrq";
map<char, int> pieceToCode;
map<int, char> codeToPiece;


// ------------------ Init Functions ------------------
void initializeCharset() {
    for (int i = 0; i < CHARSET.size(); ++i) {
        charToCode[CHARSET[i]] = i;
        codeToChar[i] = CHARSET[i];
    }
}

void initializePieceset() {
    for (int i = 0; i < PIECESET.size(); ++i) {
        pieceToCode[PIECESET[i]] = i;
        codeToPiece[i] = PIECESET[i];
    }
}

// ------------------ Core Functions ------------------

// Now returns a pair of {bits, original length}
string encodeText(string text) {
    string bits;
    for (char c : text) {
        bits += bitset<BITS_PER_CHAR>(charToCode[c]).to_string();
    }

    while (bits.size() % 29 != 0) {
        bits += '0';
    }
    return bits;
}

int bitsToInt(string bits) {
    bitset<32> bitsetNumber(bits);   // Create a bitset from the string
    int recoveredNumber = (int)bitsetNumber.to_ulong();  // Convert bitset to unsigned long, cast to int
    return recoveredNumber;
}

string chunkToPieces(int data) {
    string result;

    while (data > 0) {
        int remainder = data % 13;
        result = codeToPiece[remainder]  + result;
        data = data / 13;
    }
    while (result.size() != 8) {
        result = "1" + result;
    }
    return result;
}

string chunkToBase13(int data) {
    string digits = "0123456789abc";
    string result;

    while (data > 0) {
        int remainder = data % 13;
        result = digits[remainder]  + result;
        data = data / 13;
    }
    return result;
}

string bitsToPiece(const string& bits) {
    string pieces;

    for (int i = 0; i < bits.size(); i += 29) {
        string chunk = bits.substr(i, 29);
        cout << chunk.size() << " chunk(" << i << ',' << i + 29  << "): " << chunk;
        int bits = bitsToInt(chunk);
        string inBase13 = chunkToBase13(bits);
        string inPieces = chunkToPieces(bits);
        cout << " --> " << bits;
        cout << " --> " << inBase13;
        cout << " --> " << inPieces << endl;
        pieces += inPieces;
    }

    return pieces;
}

vector<string> piecesToBoard(string pieces) {
    vector<string> rowsVector;
    for (int i = 0; i < pieces.size(); i += 8) {
        string chunk = pieces.substr(i, 8);
        rowsVector.push_back(chunk);
    }

    while (rowsVector.size() % 8 != 0) {
        rowsVector.push_back("8");
    }

    return rowsVector;
}

string boardToFen(vector<string> rows) {
    string fenCode;

    for (string row : rows) {
        fenCode += row;
        fenCode += "/";
    }

    fenCode = fenCode.substr(0, fenCode.size() - 1);
    return fenCode;
}
void encrypt() {
    cout << "Type Message to encrypt" << endl;
    string input;
    getline(cin, input);

    string encodedBits = encodeText(input);
    cout << "\nEncoded Bits:\n" << encodedBits << endl;

    string pieces = bitsToPiece(encodedBits);

    vector<string> rows = piecesToBoard(pieces);

    // for (const string& row : rows) {
    //     cout << row << endl;
    // }

    cout << "https://lichess.org/editor/" << boardToFen(rows) << "%5B%5D_w_-_-_0_1?color=white"<<endl;

    cout << "\nPieces:\n" << pieces << endl;
}

string fenToPieces(string fencode) {
    string result;

    for (char c : fencode) {
        if (c == '/') continue;
        if (isdigit(c)) {
            int count = c -'0';
            for (int i = 0; i < count; ++i) {
                result += '1';
            }
        } else {
            result += c;
        }
    }
    return result;
}

int piecesToBase10(string pieces) {
    int result = 0;
    for (char c : pieces) {
        int value = pieceToCode[c];

        result = result * 13 + value;

    }
    return result;
}

string intToBinary(int num) {
    return bitset<29>(num).to_string();
}

string piecesToBits(string pieces) {
    string bits;

    for (int i = 0; i < pieces.size(); i += 8) {
        string chunk = pieces.substr(i, 8);
        cout << chunk.size() << " chunk(" << i << ',' << i + 8  << "): " << chunk;
        int inBase10 = piecesToBase10(chunk);
        string bin = intToBinary(inBase10);
        cout << " --> " << inBase10;
        cout << " --> " << bin << endl;

        bits += bin;
    }

    return bits;
}

int chunkToInt(string chunk) {
    bitset<5> bitsetNumber(chunk);
    return (int)bitsetNumber.to_ulong();
}

string binariesToText(string bin) {
    string result;

    for (int i = 0; i < bin.size(); i += 5) {
        string chunk = bin.substr(i, 5);
        int num = chunkToInt(chunk);
        result += codeToChar[num];
    }

    return result;
}

string urlToFencode(string url) {
    string extracted = url.substr(28, url.size() - 28 - 28);
    cout << extracted << endl;
    return extracted;

}
void decrypt() {
    cout << "Type ChessCode URL to decrypt" << endl;
    string input;
    getline(cin, input);

    // input = urlToFencode(input);

    string pieces = fenToPieces(input);
    cout << "Pieces: " << pieces << endl;

    string bin = piecesToBits(pieces);
    cout << bin << endl;

    string message = binariesToText(bin);
    cout << message << endl;

}

void handleInput() {
    while (true) {
        cout << "Type D to decrypt, E to encrypt, or B for benchmark (both)" << endl;
        string input;
        getline(cin, input);

        if (input == "E") {
            auto start = steady_clock::now();
            encrypt();
            auto end = steady_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Encryption took " << duration.count() << " μs" << endl;
            break;
        }
        else if (input == "D") {
            auto start = steady_clock::now();
            decrypt();
            auto end = steady_clock::now();
            auto duration = duration_cast<microseconds>(end - start);
            cout << "Decryption took " << duration.count() << " μs" << endl;
            break;
        }
        else if (input == "B") {
            cout << "Type Message to encrypt and decrypt for benchmarking:" << endl;
            string text;
            getline(cin, text);

            // --- Encryption ---
            auto enc_start = steady_clock::now();
            string encodedBits = encodeText(text);
            string pieces = bitsToPiece(encodedBits);
            vector<string> rows = piecesToBoard(pieces);
            string fen = boardToFen(rows);
            auto enc_end = steady_clock::now();

            cout << "\n[Encryption Completed]" << endl;
            cout << "FEN Output: " << fen << endl;

            // --- Decryption ---
            auto dec_start = steady_clock::now();
            string decodedPieces = fenToPieces(fen);
            string bin = piecesToBits(decodedPieces);
            string message = binariesToText(bin);
            auto dec_end = steady_clock::now();

            cout << "\n[Decryption Completed]" << endl;
            cout << "Decrypted Message: " << message << endl;

            // --- Timings ---
            auto enc_duration = duration_cast<microseconds>(enc_end - enc_start);
            auto dec_duration = duration_cast<microseconds>(dec_end - dec_start);
            cout << "\nEncryption Time: " << enc_duration.count() << " μs" << endl;
            cout << "Decryption Time: " << dec_duration.count() << " μs" << endl;
            break;
        }
        else {
            cout << "Invalid input" << endl;
            continue;
        }
    }
}

int main() {
    initializeCharset();
    initializePieceset();
    handleInput();
}
