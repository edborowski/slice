#include "fen.h"
#include "globals.h"

using namespace std;

CFen::CFen() {

}

string CFen::LoadFromFile(string filename) {
	ifstream file;
	string line;

	file.open(filename.c_str());
	getline(file,line);

	file.close();

	return(line);
}

void CFen::LoadFENString(string fen) {
	int i = 0;

	istringstream iss(fen);
	while (iss) { 
		string subs; 
		iss >> subs; 

		switch (i++) {
			case 0: // fen position
					position = subs;
					break;
			case 1: // to move
					color = subs;
					break;
			case 2: // castling
					castling = subs;
					break;
			case 3: // en passant
					enpassant = subs;
					break;
			case 4: // halfmove
					halfmove = atoi(subs.c_str());
					break;
			case 5: // fullmove
					fullmove = atoi(subs.c_str());
					break;
			default: break;					
		}
	} 
}

void CFen::getPosition(int chessboard[]) {
	//int chessboard[64];
	int sq = 56;
	int idx = 0;

	for (int x = 0; x < 64; x++) {
		chessboard[x] = 0;
	}

	for (unsigned int i = 0; i < position.length(); i++) {
		switch(position[i]) {
			case 'p':   // black pawn
						chessboard[sq + idx++] = BP;
						break;
			case 'n':   // black knight
						chessboard[sq + idx++] = BN;
						break;
			case 'b':   // black bishop
						chessboard[sq + idx++] = BB;
						break;
			case 'r':   // black rook
						chessboard[sq + idx++] = BR;
						break;
			case 'q':   // black queen
						chessboard[sq + idx++] = BQ;
						break;
			case 'k':   // black king
						chessboard[sq + idx++] = BK;
						break;
			case 'P':	// white pawn
						chessboard[sq + idx++] = WP;
						break;
			case 'N':	// white knight
						chessboard[sq + idx++] = WN;
						break;
			case 'B':	// white bishop
						chessboard[sq + idx++] = WB;
						break;
			case 'R':	// white rook
						chessboard[sq + idx++] = WR;
						break;
			case 'Q':	// white queen
						chessboard[sq + idx++] = WQ;
						break;
			case 'K':	// white king
						chessboard[sq + idx++] = WK;
						break;
			case '1':   // empty square
						idx++;
						break;
			case '2':	// 2 empty squares
						idx = idx + 2;
						break;
			case '3':	// 2 empty squares
						idx = idx + 3;
						break;
			case '4':	// 2 empty squares
						idx = idx + 4;
						break;
			case '5':	// 2 empty squares
						idx = idx + 5;
						break;
			case '6':	// 2 empty squares
						idx = idx + 6;
						break;
			case '7':	// 2 empty squares
						idx = idx + 7;
						break;
			case '8':	// 2 empty squares
						idx = idx + 8;
						break;
			case '/':   // rank break
						sq = sq - 16;
						break;
			default:    break;
		}
	}
}

string CFen::getCastling() {
	return (castling);
}

string CFen::getColor() {
	return (color);
}

int CFen::getEnpassant() {
	int ep = -1; 
	string epsquare[64] = {
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
	};

	for (int i = 0; i < 64; i++) {
		if (epsquare[i] == enpassant) {
			ep = i;
			break;
		}
	}
	
	// return -1 means either invalid or "-" for no enpassant
	return (ep);
}

int CFen::getHalfmove() {
	return (halfmove);
}

int CFen::getFullmove() {
	return (fullmove);
}


