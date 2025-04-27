// // chessboard_base13.cpp
// // C++ encryption and decryption based on Base-13 chessboard encoding for ~47 characters per board.
//
// #include <iostream>
// #include <vector>
// #include <string>
// #include <map>
// #include <sstream>
// #include <bitset>
// #include <cmath>
//
// using namespace std;
//
// const vector<char> whitePieces = {'P', 'N', 'B', 'K', 'R', 'Q'};
// const vector<char> blackPieces = {'p', 'n', 'b', 'k', 'r', 'q'};
// const char emptySquare = '.';
// const int BASE = 13;
// const int BITS_PER_CHAR = 5;
// const int TOTAL_SQUARES = 64;
//
// // 32-symbol custom charset
// const string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .,;?!";
// map<char, int> charToCode;
// map<int, char> codeToChar;
//
// void initializeCharset() {
//     for (int i = 0; i < charset.size(); ++i) {
//         charToCode[charset[i]] = i;
//         codeToChar[i] = charset[i];
//     }
// }
//
// // Map value 0-12 to chess piece
// char valueToPiece(int val) {
//     if (val == 0) return emptySquare;
//     if (val >= 1 && val <= 6) return blackPieces[val - 1];
//     if (val >= 7 && val <= 12) return whitePieces[val - 7];
//     return emptySquare;
// }
//
// // Map chess piece to value 0-12
// int pieceToValue(char c) {
//     if (c == emptySquare) return 0;
//     for (int i = 0; i < 6; ++i) {
//         if (c == blackPieces[i]) return i + 1;
//         if (c == whitePieces[i]) return i + 7;
//     }
//     return 0;
// }
//
// string generateFEN(const vector<int>& boardValues) {
//     string fen;
//     for (int row = 0; row < 8; ++row) {
//         int emptyCount = 0;
//         for (int col = 0; col < 8; ++col) {
//             int idx = row * 8 + col;
//             char piece = valueToPiece(boardValues[idx]);
//             if (piece == emptySquare) {
//                 emptyCount++;
//             } else {
//                 if (emptyCount > 0) {
//                     fen += to_string(emptyCount);
//                     emptyCount = 0;
//                 }
//                 fen += piece;
//             }
//         }
//         if (emptyCount > 0) fen += to_string(emptyCount);
//         if (row != 7) fen += '/';
//     }
//     return fen;
// }
//
// vector<int> encodeText(const string& text) {
//     string bits;
//     for (char c : text) {
//         bits += bitset<BITS_PER_CHAR>(charToCode[c]).to_string();
//     }
//
//     vector<int> values;
//     for (size_t i = 0; i < bits.length(); i += 4) {
//         string chunk = bits.substr(i, 4);
//         while (chunk.length() < 4) chunk += "0"; // pad last chunk
//         int val = stoi(chunk, nullptr, 2);
//         values.push_back(val % BASE);
//     }
//
//     while (values.size() < TOTAL_SQUARES) {
//         values.push_back(0); // pad remaining squares with empty
//     }
//
//     return values;
// }
//
// string decodeBoard(const vector<int>& boardValues) {
//     string bits;
//     for (int val : boardValues) {
//         bits += bitset<4>(val).to_string();
//     }
//
//     string output;
//     for (size_t i = 0; i + BITS_PER_CHAR <= bits.length(); i += BITS_PER_CHAR) {
//         string chunk = bits.substr(i, BITS_PER_CHAR);
//         int code = stoi(chunk, nullptr, 2);
//         if (codeToChar.count(code)) {
//             output += codeToChar[code];
//         }
//     }
//     return output;
// }
//
// void encrypt() {
//     cout << "Enter text to encrypt (uppercase + space . , ; ? ! only): ";
//     string input;
//     getline(cin, input);
//
//     vector<int> board = encodeText(input);
//     string fen = generateFEN(board);
//
//     cout << "\nGenerated FEN: " << fen << endl;
//     cout << "\nLichess Link: https://lichess.org/editor/" << fen << "_w_-_-_0_1?color=white" << endl;
// }
//
// void decrypt() {
//     cout << "Enter FEN (board layout only): ";
//     string fen;
//     getline(cin, fen);
//
//     vector<int> board;
//     for (char c : fen) {
//         if (c == '/') continue;
//         if (isdigit(c)) {
//             int num = c - '0';
//             for (int i = 0; i < num; ++i) {
//                 board.push_back(0);
//             }
//         } else {
//             board.push_back(pieceToValue(c));
//         }
//     }
//
//     string decoded = decodeBoard(board);
//     cout << "\nDecoded Text: " << decoded << endl;
// }
//
// int main() {
//     initializeCharset();
//
//     cout << "Type 'e' to encrypt or 'd' to decrypt: ";
//     char choice;
//     cin >> choice;
//     cin.ignore();
//
//     if (choice == 'e' || choice == 'E') {
//         encrypt();
//     } else if (choice == 'd' || choice == 'D') {
//         decrypt();
//     } else {
//         cout << "Invalid choice." << endl;
//     }
//
//     return 0;
// }