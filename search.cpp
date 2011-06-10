#include <iostream>
#include "globals.h"
#include "proto.h"

using namespace std;

vector<move_t> pv;
move_t bestmove;

int get_moves(move_t moves[]) {
	int mv_idx = 0;
	int imv = 0;
	int imoves = 0;
	int i = 0;
    move_t mv[MAX_PIECE_MOVES]; // movelist from the current square

    // generate the movelist moves for the current position
	imv = gen_pawn_moves(mv);
	for (i = 0; i < imv; i++) {
		moves[imoves++] = mv[i];
	}
	imv = gen_knight_moves(mv);
	for (i = 0; i < imv; i++) {
		moves[imoves++] = mv[i];
	}
	imv = gen_king_moves(mv);
	for (i = 0; i < imv; i++) {
		moves[imoves++] = mv[i];
	}
	imv = gen_rook_moves(mv);
	for (i = 0; i < imv; i++) {
		moves[imoves++] = mv[i];
	}
	imv = gen_bishop_moves(mv);
	for (i = 0; i < imv; i++) {
		moves[imoves++] = mv[i];
	}

	return imoves;
}

// todo:  if game < 5 min keep simple
// todo:  if game > 5 min... gen root nodes, go 3 ply, reorder, go deeper (iterative deepening)
move_t movegen(int depth) {
	int imoves = 0;
    move_t moves[MAX_MOVES]; // movelist from the current position

	// initialize bestmove
	bestmove.score = -INFINITY;
	bestmove.src = -1;
	bestmove.dst = -1;

	imoves = get_moves(moves);

	for (int x = 0; x < imoves; x++) {
		makemove(moves[x]);
		pv.push_back(moves[x]);
		moves[x].score = -alphabeta(depth-1, -INFINITY, INFINITY);
		takeback(moves[x]);
		pv.pop_back();

		if (moves[x].score > bestmove.score) {
			bestmove = moves[x];
		} 
        cout << move_to_long_algebraic(moves[x]) << " " << moves[x].score << endl;
	}

	cout << "bestmove: " << move_to_long_algebraic(bestmove) << " " << bestmove.score << endl;

	return (bestmove);
}

//move_t movegen(int depth) {
//	int imoves = 0; // position move counter
//    int u = 0;
    //vector<move_t> moves; // movelist from the current position
//    move_t moves[MAX_MOVES]; // movelist from the current position
    //vector<move_t> mv; // movelist from the current square
//    move_t m;
//    move_t best;

//	imoves = get_moves(moves);

    // iterate through the movelist moves
    //for (u = 0; u < moves.size(); u++) {
//    for (u = 0; u < imoves; u++) {
//        if (depth == 0) {
//            makemove(moves[u]);
//            moves[u].score = eval();
//            takeback(moves[u]);
//        } else {
//            makemove(moves[u]);
//            m = movegen(depth - 1);
//            takeback(moves[u]);
//            moves[u].score = m.score;
//        }

        // calculate best move (min/max)
        // ie: pull back highest white score
        // ie: pull back lowest black score
//        if (u == 0) {
//            best = moves[u];
//        } else {
//            if (white_to_move) {
//                if (moves[u].score > best.score) {
//                    best = moves[u];
//                } else if (moves[u].score == best.score) {
//                    if (coin_toss()) {
//                        best = moves[u];
//                    }
//                }
//            } else { // black to move
//                if (moves[u].score < best.score) {
//                    best = moves[u];
//                } else if (moves[u].score == best.score) {
//                    if (coin_toss()) {
//                        best = moves[u];
//                    }
//                }
//            }
//        }
//    }

    // return the best move
//    return (best);
//}

// alphabeta
// call with alphabeta(search_depth, INFINITY, -INFINITY) to start
int alphabeta(int depth, int alpha, int beta) {
	int pval = 0;
	//int legal = 0;
	int imoves = 0;
	//int best = -INFINITY;
    move_t moves[MAX_MOVES]; // movelist from the current position

	if (depth == 0) {
		if ((search_depth & 1) == 1) {
			pval = -eval();
		} else {
			pval = eval();
		}

		// decent print but too much info for gui
		pv[pv.size()-1].score = pval;
		//print_variation(pv);

		return (pval);
	}

	imoves = get_moves(moves);

	for (int x = 0; x < imoves; x++) {
		//if (best > beta) { break; }
		makemove(moves[x]);
		pv.push_back(moves[x]);
		pval = -alphabeta(depth-1, -beta, -alpha);
		moves[x].score = pval;
		takeback(moves[x]);

		if (pv.size() > 0) {
			pv[pv.size()-1].score = pval;
		//	print_variation(pv);
		}

		pv.pop_back();
		if (pval >= beta) { /* cout << "^^^^^ CUTOFF pval(" << pval << ") >= beta(" << beta << ")" << endl; */ return (beta); }
		if (pval > alpha) {
			//cout << "^^^^^ NEW BEST MOVE pval(" << pval << ") > alpha(" << alpha << ")" << endl;
			//if (depth == search_depth) {
			//	bestmove = moves[x];
			//}
			//todo: pondering?
			//if (pv.size() > 0) {
			//	pv[pv.size()-1].score = pval;
			//	print_variation(pv);
			//}
			alpha = pval; 
		} 
	}

	return (alpha);
}

// SCRAP CODE FOR PAWN GENERATION NON-BITBOARD
		/*
        for (x = 0; x < 64; x++) {
            if (white_to_move) {
				if (chessboard[x] == WP) {
					imv = pawngenw(x, mv);
					for (i = 0; i < imv; i++) {
						if (mv[i].dst < 0) {
							cout << "white oops" << endl;
						}
						moves[imoves++] = mv[i];
					}
				}
            }
            if (!white_to_move) {
				if (chessboard[x] == BP) {
					imv = pawngenb(x, mv);
				    for (i = 0; i < imv; i++) {
						if (mv[i].dst < 0) {
							cout << "black oops" << endl;
						}
						moves[imoves++] = mv[i];
		            }
				}
            }
        }
		*/
