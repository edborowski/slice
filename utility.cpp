#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "globals.h"
#include "proto.h"

// used for bitScanForward
const int index64[64] = {
   63,  0, 58,  1, 59, 47, 53,  2,
   60, 39, 48, 27, 54, 33, 42,  3,
   61, 51, 37, 40, 49, 18, 28, 20,
   55, 30, 34, 11, 43, 14, 22,  4,
   62, 57, 46, 52, 38, 26, 32, 41,
   50, 36, 17, 19, 29, 10, 13, 21,
   56, 45, 25, 31, 35, 16,  9, 12,
   44, 24, 15,  8, 23,  7,  6,  5
};

void print_chessboard() {
    int x, y, s;

    cout << "------------------------------" << endl;
    cout << "move: " << move_no << " ply: " << ply << endl;

    if (flip) {
        cout << "(white)" << endl;
    } else {
        cout << "(black)" << endl;
    }

    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++)  {
			s = (8 * x) + y;
			if (!flip) {
                s = 63 - s - 7 + y + y;
            }
            switch (chessboard[s]) {
				case WP: cout << "P"; break;
                case WN: cout << "N"; break;
                case WB: cout << "B"; break;
                case WR: cout << "R"; break;
                case WQ: cout << "Q"; break;
				case WK: cout << "K"; break;
                case BP: cout << "p"; break;
                case BN: cout << "n"; break;
                case BB: cout << "b"; break;
                case BR: cout << "r"; break;
                case BQ: cout << "q"; break;
				case BK: cout << "k"; break;
                default: cout << "-"; break;
            }
        }
        cout << endl;
    }

    if (flip) {
        cout << "(black)" << endl;
    } else {
        cout << "(white)" << endl;
    }

    if (move_list.empty()) {
        cout << "- starting position -" << endl;
    } else {
       cout << "Position after " << move_to_long_algebraic(move_list[move_list.size()-1]) << endl;
		cout << "SCORE: " << move_list[move_list.size()-1].score << endl;
	   //cout << "numeric after " << move_list[move_list.size()-1].src << "-" << move_list[move_list.size()-1].dst << endl;
    }

    if (white_to_move) { cout << "WHITE TO MOVE" << endl; }
    else { cout << "BLACK TO MOVE" << endl; }

	//if (enpassant) {
	//	cout << "ENPASSANT: " << enpassant << " square: " << boardsquares[epsquare] << endl;
	//}
}

string move_to_long_algebraic(move_t m) {
    string str;

	switch(m.piece) {
		case WP: break;
		case WN: str = "N"; break;
		case WB: str = "B"; break;
		case WR: str = "R"; break;
		case WQ: str = "Q"; break;
		case WK: str = "K"; break;
		case BP: break;
		case BN: str = "N"; break;
		case BB: str = "B"; break;
		case BR: str = "R"; break;
		case BQ: str = "Q"; break;
		case BK: str = "K"; break;
		default: break;
	}

    str += boardsquares[m.src];
    if (m.capture == 0) {
        str += "-";
    } else {
        str += "x";
    }
    str += boardsquares[m.dst];
    
    if ((m.piece == WK) || (m.piece == BK)) {
        if (m.dst == m.src + 1) {
            str = "O-O";
        } else if (m.dst == m.src - 2) {
            str = "O-O-O";
        }
    }

    return str;
}

void print_vector(vector<move_t> v) {
    for (unsigned int i = 0; i < v.size(); i++) {
        cout << v[i].src << "-" << v[i].dst << " (" << v[i].score << ")" << endl;
    }
}

void print_variation(vector<move_t> v) {
	for (unsigned int i = 0; i < v.size(); i++) {
        if (i == 0) {
            if (evaluating) {
                cout << "evaluating: ";
            } else {
                cout << "moving: ";
            }
            if (!v[i].white_to_move) {
                cout << v[i].move_no << ". ...";
            } 
        }
        if (v[i].white_to_move) {
            cout << v[i].move_no << ".";
        }
        cout << " " << move_to_long_algebraic(v[i]);
		cout << "(" << v[i].score << ")";
		cout << " ";
    }
    cout << endl;
	//print_chessboard();
}

void print_movelist() {
    for (unsigned int i = 0; i < move_list.size(); i++) {
        if (i == 0) {
            if (!move_list[i].white_to_move) {
                cout << move_list[i].move_no << ". ...";
            } 
        }
        if (move_list[i].white_to_move) {
            cout << move_list[i].move_no << ".";
        }
        cout << " " << move_to_long_algebraic(move_list[i]) << " ";
    }
    cout << endl;
}

int coin_toss() {
    int result;

    if (cointoss) {
        cointoss = false;
        srand((unsigned)time(NULL));
    }
    result = (rand() % 2);
    return result;
}

void print_bitboard(string s, BITBOARD bb) {
	int r,f;

	cout << "--------------------------------" << endl;
	cout << s << " [" << hex << bb << "]" << endl;
	cout << "--------------------------------" << endl;
	
	// r denotes rank, f denotes file		
	for (r=7; r>=0; r--) {
		for (f=0; f<8; f++) {
			BITBOARD bitmask = 1;
		
			if ((bb & (bitmask << (r*8+f))) > 0) {
				cout << 1;
            } else {
				cout << 0;
			}				
		}
	
		cout << endl;
    }
}

void print_bitboard_long(string s, BITBOARD bb) {
	int r,f;

	cout << s << ":";
	
	// r denotes rank, f denotes file		
	for (r=7; r>=0; r--) {
		for (f=0; f<8; f++) {
			BITBOARD bitmask = 1;
		
			if ((bb & (bitmask << (r*8+f))) > 0) {
				cout << 1;
            } else {
				cout << 0;
			}				
		}
    }
	cout << endl;
}

// returns -1 if invalid / not found
int square_to_int(string s) {
	int square = -1;

	for (int x=0; x<64; x++) {
		if (chessboard_squares[x] == s) {
			square = x;
			break;
		}
	}

	return square;
}

/**
 * bitScanForward
 * @author Martin Läuter (1997)
 *         Charles E. Leiserson
 *         Harald Prokop
 *         Keith H. Randall
 * "Using de Bruijn Sequences to Index a 1 in a Computer Word"
 * @param bb bitboard to scan
 * @precondition bb != 0 -- removed from code by EB
 * @return index (0..63) of least significant one bit
 */
int bitScanForward(BITBOARD bb) {
   const BITBOARD debruijn64 = 0x07EDD5E59A4E28C2;
   return index64[((bb & -bb) * debruijn64) >> 58];
}
