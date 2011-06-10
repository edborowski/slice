#ifndef FEN_H_
#define FEN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class CFen {
	public:
		CFen();
		string LoadFromFile(string);
		void LoadFENString(string);
		void getPosition(int[]);
		string getColor();
		string getCastling();
		int getEnpassant();
		int getHalfmove();
		int getFullmove();
	private:
		string position;
		string color; // who moves next, "w" or "b"
		string castling; // castling availability "KQkq" or "-"
		string enpassant; // target square in algebraic e4 would be e3.. or "-"
		int halfmove; // since last pawn advance or capture for 50-move rule
		int fullmove; // starts at 1, incremented after black move
};

#endif //FEN_H_