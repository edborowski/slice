/* eval():
 * For the position on the board, return a score
 * 1.  Piece Count/Value
 * 2.  Piece/Square Modifiers
 */
#include <iostream>
#include "globals.h"

using namespace std;

static int pawnvaluew[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    10, 10, 10, 10, 10, 10, 10, 10,
    10, 10, 20, 20, 20, 10, 10, 10,
    20, 20, 30, 30, 30, 20, 20, 20,
    30, 30, 40, 40, 40, 30, 30, 30,
    50, 50, 50, 50, 50, 50, 50, 50,
    900, 900, 900, 900, 900, 900, 900, 900
};

static int pawnvalueb[64] = {
    900, 900, 900, 900, 900, 900, 900, 900,
    50, 50, 50, 50, 50, 50, 50, 50,
    30, 30, 40, 40, 40, 30, 30, 30,
    20, 20, 30, 30, 30, 20, 20, 20,
    10, 10, 20, 20, 20, 10, 10, 10,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

static int nightvalue[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 10, 20, 20, 20, 20, 10, 0,
	0, 10, 50, 20, 20, 50, 10, 0,
	0, 10, 20, 20, 20, 20, 10, 0,
	0, 10, 20, 20, 20, 20, 10, 0,
	0, 10, 50, 20, 20, 50, 10, 0,
	0, 10, 20, 20, 20, 20, 10, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static int bishopvalue[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 50, 0, 50, 50, 0, 50, 0,
	25, 0, 25, 25, 25, 25, 0, 25,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	25, 0, 25, 25, 25, 25, 0, 25,
	0, 50, 0, 50, 50, 0, 50, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

int eval(void) {
    int wpscore = 0; //White Piece Score
    int wmscore = 0; //White Modifier Score
    int bpscore = 0; //Black Piece Score
    int bmscore = 0; //Black Modifier Score
    int score = 0; //Position Score
	int white_king_score = 0;
	int black_king_score = 0;
	bool white_king_missing = true;
	bool black_king_missing = true;

    for (int x = 0; x < 64; x++) {
		switch(chessboard[x]) {
			case WP: wpscore += 100; wmscore += pawnvaluew[x]; break;
			case WN: wpscore += 300; wmscore += nightvalue[x]; break;
			case WB: wpscore += 300; wmscore += bishopvalue[x]; break;
			case WR: wpscore += 500; break;
			case WQ: wpscore += 900; break;
			case WK: wpscore += 1800; white_king_missing = false; break;
			case BP: bpscore += 100; bmscore += pawnvalueb[x]; break;
			case BN: bpscore += 300; bmscore += nightvalue[x]; break;
			case BB: bpscore += 300; bmscore += bishopvalue[x]; break;
			case BR: bpscore += 500; break;
			case BQ: bpscore += 900; break;
			case BK: bpscore += 1800; black_king_missing = false; break;
			default: break;
		}
    }

	if (white_king_missing) { white_king_score = INFINITY; }
	if (black_king_missing) { black_king_score = INFINITY; }

	// add black king to white, and white king to black since it's desirable for the alternate.
	// if white_to_move, it's black's score because black just moved
	if (white_to_move) {
	    score = bpscore + bmscore + white_king_score - wpscore - wmscore - black_king_score;
	    //score = bpscore + white_king_score - wpscore -  black_king_score;
		//cout << "black score:" << score << " wp:" << wpscore << " bk:" << black_king_score << " bp:" << bpscore << " wk:" << white_king_score << endl;
	} else {
		score = wpscore + wmscore + black_king_score - bpscore - bmscore - white_king_score;
		//score = wpscore + black_king_score - bpscore - white_king_score;
		//cout << "white score:" << score << " wp:" << wpscore << " bk:" << black_king_score << " bp:" << bpscore << " wk:" << white_king_score << endl;
	}
    return score;
}

