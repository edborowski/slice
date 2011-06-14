// todo:
// scoring moves at end of line but not at maxply yet (if no moves or if m.piece=K/k in alphabeta)
// understand win/lose/draw
// (0.1.1) improve print_chessboard
// (0.1.1) save FEN position
// (0.1.1) add save to save in pgn
// (0.1.1) add open to load from pgn
// consider testing routine for new pieces (pgn/fen/print/score/...)
// (0.1.1) current_variation cleanup (switch to array?)
// (0.1.1) move_list cleanup (switch to array?)
// add support for fen in uci position command
// (0.1.1) add quiescent search
// (0.1.1) add iterative deepening
// (0.1.1) add support for xboard/winboard
// update search/eval to support check by score
// update search/eval to support checkmate by score
// update search/eval to support stalemate by score
// (0.1.1) check why splitting search.cpp get_moves() slowed search from 1.2m to 800k
// (0.1.1) try running gprof
// (0.1.1) add standard test for nps performance
// (0.1.1) add support for epd
// add h/help/? to show all available commands
// add support to show pv in gui
// add support to show pondering in gui
// if validate castling, save top 3 moves so that 1 won't be castling and validate at bestmove if using attack bitboards
// support 50-move rule
// support draw by repetition
// (0.1.1) add mobility to eval() function (could just save move count wmc and bmc from movelist in alphabeta)
// (0.1.x) add nicer interface for 'd'
// update bitboards properly when castling
// update can't castle through check
// update can't castle if king moves - test
// update can't castle if rook moves
// update if gui sends castling e1g1 ex how to fix
// (0.1.1) add opening book
// (0.1.1) add transposition tables
// (0.1.1) add zobrist keys
// time management / time controls
// update king can't castle once king moves (test)
// bug: black sometimes gets an extra rook on the f8 square maybe due to O-O in search

/*
 *	UCI Commands Supported
 *	quit - Exit the program
 *  uci - send program info
 *  ucinewgame - new game
 *  go - evaluate the current position, return bestmove (not yet implemented)
 *  isready - reply readyok
 *  to be implemeneted: position
 *  debug - turn debug on/off - send debug info with the info string command
 *  register - example "register later" or "register name Stefan MK code 4359874324"
 *  position - get the position and moves from the engine
 */

#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <string>

#include "globals.h"
#include "proto.h"
#include "fen.h"

#ifdef _WIN32
	#include "windows.h"
#endif 

using namespace std;

bool isnewgame = false;

//http://cboard.cprogramming.com/cplusplus-programming/91892-how-rotate-64-bit-bitboard.html
//http://chessprogramming.wikispaces.com/Square+Mapping+Considerations
//http://www.mayothi.com/nagaskakichess6.html


void new_game(string fen) {
	BITBOARD bb = 1;
    white_to_move = true;
    nodes = 0;
    move_no = 1;
	ply = 0;
	enpassant = false;
	epsquare = -1;
	evaluating = false;
	flip = false;

	current_variation.clear();
	move_list.clear();
	bb_white_pawns = 0;
	bb_white_knights = 0;
	bb_white_bishops = 0;
	bb_white_rooks = 0;
	bb_white_queens = 0;
	bb_white_king = 0;
	bb_black_pawns = 0;
	bb_black_knights = 0;
	bb_black_bishops = 0;
	bb_black_rooks = 0;
	bb_black_queens = 0;
	bb_black_king = 0;
	bb_white_pieces = 0;
	bb_black_pieces = 0;
	bb_all_pieces = 0;
	bb_all_pieces90 = 0;
	bb_all_pieces45l = 0;
	bb_all_pieces45r = 0;

	CFen cfenload = CFen();
	cfenload.LoadFENString(fen);
    cfenload.getPosition(chessboard);  // fills "chessboard"
    string castling = cfenload.getCastling();
    // Castling
    white_can_castle_kingside = false;
    white_can_castle_queenside = false;
    black_can_castle_kingside = false;
    black_can_castle_queenside = false;
    for (unsigned int x = 0; x < castling.size(); x++) {
	    if (castling.at(x) == '-') { break; }
        if (castling.at(x) == 'K') { white_can_castle_kingside = true; }
        if (castling.at(x) == 'Q') { white_can_castle_queenside = true; }
        if (castling.at(x) == 'k') { black_can_castle_kingside = true; }
        if (castling.at(x) == 'q') { black_can_castle_queenside = true; }
    }
	if (cfenload.getColor() == "w") {
        white_to_move = true;
    } else {
		white_to_move = false;
    }
	if ((epsquare = cfenload.getEnpassant()) >= 0) {
        enpassant = true;
    } else {
        enpassant = false;
    }
	move_no = cfenload.getFullmove();
	half_move = cfenload.getHalfmove();

    for (int x = 0; x < 64; x++) {
        //chessboard[x] = startpos[x];
		if (chessboard[x] != 0) {
			bb_all_pieces90 = bb_all_pieces90 | (bb << rotate90[x]);
			bb_all_pieces45l = bb_all_pieces45l | (bb << rotate45l[x]);
			bb_all_pieces45r = bb_all_pieces45r | (bb << rotate45r[x]);
		}
		switch(chessboard[x]) {
			//todo: can try | with bb_squares[x] instead of shifting but speed doesn't matter here
			case WP: bb_white_pawns = bb_white_pawns | (bb << x); break;
			case WN: bb_white_knights = bb_white_knights | (bb << x); break;
			case WB: bb_white_bishops = bb_white_bishops | (bb << x); break;
			case WR: bb_white_rooks = bb_white_rooks | (bb << x); break;
			case WQ: bb_white_queens = bb_white_queens | (bb << x); break;
			case WK: bb_white_king = bb_white_king | (bb << x); break;
			case BP: bb_black_pawns = bb_black_pawns | (bb << x); break;
			case BN: bb_black_knights = bb_black_knights | (bb << x); break;
			case BB: bb_black_bishops = bb_black_bishops | (bb << x); break;
			case BR: bb_black_rooks = bb_black_rooks | (bb << x); break;
			case BQ: bb_black_queens = bb_black_queens | (bb << x); break;
			case BK: bb_black_king = bb_black_king | (bb << x); break;
			default: break;
		}
    }    
	bb_white_pieces = bb_white_pawns | bb_white_knights | bb_white_bishops | bb_white_rooks | bb_white_queens | bb_white_king;
	bb_black_pieces = bb_black_pawns | bb_black_knights | bb_black_bishops | bb_black_rooks | bb_black_queens | bb_black_king;
	bb_all_pieces = bb_white_pieces | bb_black_pieces;
}

/* makemove():
 * Make a move on the chessboard
 */
void makemove(move_t m) {
	assert((m.src >= 0) && (m.src < 64));
	assert((m.dst >= 0) && (m.dst < 64));
	assert((m.piece <= WK) && (m.piece >= BK));
	assert((m.capture <= WK) && (m.capture >= BK));
	assert((m.epsquare >= -1) && (m.epsquare <= 64));

    chessboard[m.src] = 0;
    chessboard[m.dst] = m.piece;
    white_to_move = m.white_to_move;
    ply = m.ply;
    move_no = m.move_no;
	
	// it was an ep capture by a pawn
	if ((m.dst == m.epsquare) && ((m.piece == WP) || (m.piece == BP))) {
		if (m.capture == BP) {
			chessboard[m.dst+8] = 0; // remove the enpassant pawn
			bb_black_pawns = (bb_black_pawns & (~bb_squares[m.dst+8]));
			bb_all_pieces90 = bb_all_pieces90 & (~bb_squares[rotate90[m.dst+8]]);
			bb_all_pieces45l = bb_all_pieces45l & (~bb_squares[rotate45l[m.dst+8]]);
			bb_all_pieces45r = bb_all_pieces45r & (~bb_squares[rotate45r[m.dst+8]]);
		} else if (m.capture == WP) {
			chessboard[m.dst-8] = 0;
			bb_white_pawns = (bb_white_pawns & (~bb_squares[m.dst-8]));
			bb_all_pieces90 = bb_all_pieces90 & (~bb_squares[rotate90[m.dst-8]]);
			bb_all_pieces45l = bb_all_pieces45l & (~bb_squares[rotate45l[m.dst-8]]);
			bb_all_pieces45r = bb_all_pieces45r & (~bb_squares[rotate45r[m.dst-8]]);
		} else {
			// this should never happen
			cout << "info string [error] INVALID ENPASSANT CAPTURE PIECE m.capture=" << m.capture << endl;
        }
    }
	
	// Was it a castling move?  If so the king should be good but the rook needs to be updated
	if (m.piece == WK) {
		// if the white king moved, white can't castle anymore
		white_can_castle_kingside = false;
		white_can_castle_queenside = false;
		if (m.dst == m.src - 2) {
			// queenside castle
			chessboard[m.dst - 2] = 0;
			chessboard[m.dst + 1] = WR;
			bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst-2])) | bb_squares[m.dst+1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst-2]])) | bb_squares[rotate90[m.dst+1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst-2]])) | bb_squares[rotate45l[m.dst+1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst-2]])) | bb_squares[rotate45r[m.dst+1]];
		} else if (m.dst == m.src + 2) {
			// kingside castle
			chessboard[m.dst + 1] = 0;
			chessboard[m.dst - 1] = WR;
			bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst+1])) | bb_squares[m.dst-1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst+1]])) | bb_squares[rotate90[m.dst-1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst+1]])) | bb_squares[rotate45l[m.dst-1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst+1]])) | bb_squares[rotate45r[m.dst-1]];
		}
	} else if (m.piece == BK) {
		black_can_castle_kingside = false;
		black_can_castle_queenside = false;
		if (m.dst == m.src - 2) {
			// queenside castle
			chessboard[m.dst - 2] = 0;
			chessboard[m.dst + 1] = BR;
			bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst-2])) | bb_squares[m.dst+1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst-2]])) | bb_squares[rotate90[m.dst+1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst-2]])) | bb_squares[rotate45l[m.dst+1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst-2]])) | bb_squares[rotate45r[m.dst+1]];
		} else if (m.dst == m.src + 2) {
			// kingside castle
			chessboard[m.dst + 1] = 0;
			chessboard[m.dst - 1] = BR;
			bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst+1])) | bb_squares[m.dst-1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst+1]])) | bb_squares[rotate90[m.dst-1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst+1]])) | bb_squares[rotate45l[m.dst-1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst+1]])) | bb_squares[rotate45r[m.dst-1]];
		}
	}

	// set the ep flag if it was a 2 rank pawn move
	epsquare = -1;
	if (m.piece == WP) {
		if ((m.dst - m.src) == 16) {
			epsquare = m.src + 8;
		}
	} else if (m.piece == BP) {
		if ((m.src - m.dst) == 16) {
			epsquare = m.src - 8;
		}
	}

	// update bitboards
	switch (m.piece) {
		case WP: bb_white_pawns = (bb_white_pawns & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case WN: bb_white_knights = (bb_white_knights & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case WB: bb_white_bishops = (bb_white_bishops & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case WR: bb_white_rooks = (bb_white_rooks & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case WQ: bb_white_queens = (bb_white_queens & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case WK: bb_white_king = (bb_white_king & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BP: bb_black_pawns = (bb_black_pawns & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BN: bb_black_knights = (bb_black_knights & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BB: bb_black_bishops = (bb_black_bishops & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BR: bb_black_rooks = (bb_black_rooks & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BQ: bb_black_queens = (bb_black_queens & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		case BK: bb_black_king = (bb_black_king & (~bb_squares[m.src])) | bb_squares[m.dst]; break;
		default: break;
	}
	switch (m.capture) {
		// any difference for ep capture?
		case WP: bb_white_pawns = (bb_white_pawns & (~bb_squares[m.dst])); break;
		case WN: bb_white_knights = (bb_white_knights & (~bb_squares[m.dst])); break;
		case WB: bb_white_bishops = (bb_white_bishops & (~bb_squares[m.dst])); break;
		case WR: bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst])); break;
		case WQ: bb_white_queens = (bb_white_queens & (~bb_squares[m.dst])); break;
		case WK: bb_white_king = (bb_white_king & (~bb_squares[m.dst])); break;
		case BP: bb_black_pawns = (bb_black_pawns & (~bb_squares[m.dst])); break;
		case BN: bb_black_knights = (bb_black_knights & (~bb_squares[m.dst])); break;
		case BB: bb_black_bishops = (bb_black_bishops & (~bb_squares[m.dst])); break;
		case BR: bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst])); break;
		case BQ: bb_black_queens = (bb_black_queens & (~bb_squares[m.dst])); break;
		case BK: bb_black_king = (bb_black_king & (~bb_squares[m.dst])); break;
		default: break;
	}
	bb_white_pieces = bb_white_pawns | bb_white_knights | bb_white_bishops | bb_white_rooks | bb_white_queens | bb_white_king;
	bb_black_pieces = bb_black_pawns | bb_black_knights | bb_black_bishops | bb_black_rooks | bb_black_queens | bb_black_king;
	bb_all_pieces = (bb_all_pieces & (~bb_squares[m.src])) | bb_squares[m.dst];
	bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.src]])) | bb_squares[rotate90[m.dst]];
	bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.src]])) | bb_squares[rotate45l[m.dst]];
	bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.src]])) | bb_squares[rotate45r[m.dst]];

    white_to_move = !white_to_move;
    ply++;
    if (white_to_move) { move_no++; }

    // or maybe >= search_depth
    //if (ply > search_depth) {
    //    m.score = eval();
    //}
	if (evaluating) {
		current_variation.push_back(m);
		//this is probably better in alphabeta to get score at end node
		//if (debug) { 
		//	cout << "info string [debug] ";
		//	print_variation(current_variation); 
		//}
		nodes++;
	} else {
		move_list.push_back(m);
	}
}

/* takeback():
 * Take back the last move made
 */
void takeback(move_t m) {
	int offset = 0;

	if (evaluating) {
		current_variation.pop_back();
	} else {
		move_list.pop_back();
	}

    chessboard[m.src] = m.piece;
	if (m.epsquare == m.dst) {
		if (white_to_move) {
			offset = 8;
		} else {
			offset = -8;
		}
		chessboard[m.dst] = 0; // it was ep, so clear the dst
	}
	chessboard[m.dst + offset] = m.capture; 

	white_to_move = m.white_to_move;
    ply = m.ply;
    move_no = m.move_no;
	epsquare = m.epsquare;
	
	// Was it a castling move?  If so the king should be good but the rook needs to be updated
	if (m.piece == WK) {
		if (m.dst == m.src - 2) {
			// queenside castle
			chessboard[m.dst - 2] = WR;
			chessboard[m.dst + 1] = 0;
			bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst+1])) | bb_squares[m.dst-2];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst+1]])) | bb_squares[rotate90[m.dst-2]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst+1]])) | bb_squares[rotate45l[m.dst-2]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst+1]])) | bb_squares[rotate45r[m.dst-2]];
		} else if (m.dst == m.src + 2) {
			// kingside castle
			chessboard[m.dst + 1] = 0;
			chessboard[m.dst - 1] = WR;
			bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst-1])) | bb_squares[m.dst+1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst-1]])) | bb_squares[rotate90[m.dst+1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst-1]])) | bb_squares[rotate45l[m.dst+1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst-1]])) | bb_squares[rotate45r[m.dst+1]];
			white_can_castle_kingside = false;
			white_can_castle_queenside = false;
		}
	} else if (m.piece == BK) {
		if (m.dst == m.src - 2) {
			// queenside castle
			chessboard[m.dst - 2] = 0;
			chessboard[m.dst + 1] = BR;
			bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst+1])) | bb_squares[m.dst-2];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst+1]])) | bb_squares[rotate90[m.dst-2]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst+1]])) | bb_squares[rotate45l[m.dst-2]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst+1]])) | bb_squares[rotate45r[m.dst-2]];
			black_can_castle_kingside = false;
			black_can_castle_queenside = false;
		} else if (m.dst == m.src + 2) {
			// kingside castle
			chessboard[m.dst + 1] = 0;
			chessboard[m.dst - 1] = BR;
			bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst-1])) | bb_squares[m.dst+1];
			bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst-1]])) | bb_squares[rotate90[m.dst+1]];
			bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst-1]])) | bb_squares[rotate45l[m.dst+1]];
			bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst-1]])) | bb_squares[rotate45r[m.dst+1]];
			black_can_castle_kingside = false;
			black_can_castle_queenside = false;
		}
	}
	white_can_castle_kingside = m.white_can_castle_kingside;
	white_can_castle_queenside = m.white_can_castle_queenside;
	black_can_castle_kingside = m.black_can_castle_kingside;
	black_can_castle_queenside = m.black_can_castle_queenside;

	// update bitboards
	switch (m.piece) {
		case WP: bb_white_pawns = (bb_white_pawns & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case WN: bb_white_knights = (bb_white_knights & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case WB: bb_white_bishops = (bb_white_bishops & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case WR: bb_white_rooks = (bb_white_rooks & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case WQ: bb_white_queens = (bb_white_queens & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case WK: bb_white_king = (bb_white_king & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BP: bb_black_pawns = (bb_black_pawns & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BN: bb_black_knights = (bb_black_knights & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BB: bb_black_bishops = (bb_black_bishops & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BR: bb_black_rooks = (bb_black_rooks & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BQ: bb_black_queens = (bb_black_queens & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		case BK: bb_black_king = (bb_black_king & (~bb_squares[m.dst])) | bb_squares[m.src]; break;
		default: break;
	}
	switch (m.capture) {
		case WP: bb_white_pawns |= bb_squares[m.dst + offset]; break;
		case WN: bb_white_knights |= bb_squares[m.dst]; break;
		case WB: bb_white_bishops |= bb_squares[m.dst]; break;
		case WR: bb_white_rooks |= bb_squares[m.dst]; break;
		case WQ: bb_white_queens |= bb_squares[m.dst]; break;
		case WK: bb_white_king |= bb_squares[m.dst]; break;
		case BP: bb_black_pawns |= bb_squares[m.dst + offset]; break;
		case BN: bb_black_knights |= bb_squares[m.dst]; break;
		case BB: bb_black_bishops |= bb_squares[m.dst]; break;
		case BR: bb_black_rooks |= bb_squares[m.dst]; break;
		case BQ: bb_black_queens |= bb_squares[m.dst]; break;
		case BK: bb_black_king |= bb_squares[m.dst]; break;
		default: break;
	}
	bb_white_pieces = bb_white_pawns | bb_white_knights | bb_white_bishops | bb_white_rooks | bb_white_queens | bb_white_king;
	bb_black_pieces = bb_black_pawns | bb_black_knights | bb_black_bishops | bb_black_rooks | bb_black_queens | bb_black_king;
	bb_all_pieces = bb_white_pieces | bb_black_pieces;
	bb_all_pieces90 = (bb_all_pieces90 & (~bb_squares[rotate90[m.dst]])) | bb_squares[rotate90[m.src]];
	bb_all_pieces45l = (bb_all_pieces45l & (~bb_squares[rotate45l[m.dst]])) | bb_squares[rotate45l[m.src]];
	bb_all_pieces45r = (bb_all_pieces45r & (~bb_squares[rotate45r[m.dst]])) | bb_squares[rotate45r[m.src]];
	if (m.capture != 0) {
		bb_all_pieces90 |= bb_squares[rotate90[m.dst + offset]];
		bb_all_pieces45l |= bb_squares[rotate45l[m.dst + offset]];
		bb_all_pieces45r |= bb_squares[rotate45r[m.dst + offset]];
	}
}

void generate_static_bitboards() {
	gen_static_squares_bitboards();
	gen_static_king_bitboards();
	gen_static_knight_bitboards();
	gen_static_slidingh_bitboards();
	gen_static_slidingv_bitboards();
	gen_static_sliding45l_bitboards();
	gen_static_sliding45r_bitboards();
}

// if isnewgame=true we process entire movelist from position uci command
// otherwise we only process last move
void process_moves(vector<string> v) {
	move_t m;
	int s = 0;
	bool process_moves = false;

	if (!isnewgame) {
		// if it's not a new game, we only process the last move
		s = v.size() - 1;
		process_moves = true;
	}
	
	for (int x=s; x<(int)v.size(); x++) {
		if (process_moves) {
			m.src = square_to_int(v[x].substr(0,2));
			if (m.src < 0) { cout << "info string [error] invalid move " << v[x] << endl; }

			m.dst = square_to_int(v[x].substr(2,2));
			if (m.dst < 0) { cout << "info string [error] invalid move " << v[x] << endl; }

			m.white_to_move = white_to_move;
			m.piece = chessboard[m.src];
			m.capture = chessboard[m.dst];
			m.ply = ply;
			m.move_no = move_no;
			m.epsquare = epsquare;

			makemove(m);
		}
		if (v[x] == "moves") { process_moves = true; }
	}
}

int main(int argc, char *argv[])
{
	string input;
	string fenfilename;
	long start, end;
    move_t m;
	int nps;
	vector<string> vinput;

	debug = 0;
	protocol = "local";
	search_depth = 3; // 5(4 seconds 1M) 6(55 seconds 12M) 
	
	cout.setf(ios::unitbuf);  // auto flush output so buffering doesn't cause an issue with engine

    cout << "SLiCE (Developer Build)" << endl;
	cout << "The Simple Little Chess Engine" << endl;
    cout << "(c)2011 by Edwin Borowski" << endl;
	cout << "compiled " << __DATE__ << " " << __TIME__ << endl;
    cout << "------------------------------" << endl;
	
	generate_static_bitboards();
    new_game(fen_start_pos);
	
	while (1) {
	
		if (protocol == "local") { cout << "slice> "; }
		getline(cin, input);

		if (input.empty()) { input = "<noop>"; }

		if (debug) { cout << endl << "info string [debug] " << input << endl; }

		vinput.clear();
		istringstream iss(input);
		while (iss) { 
			string subs; 
			iss >> subs; 
			if (subs.length() > 0) {
				vinput.push_back(subs);
			}
		}

		if ((vinput[0] == "quit") || (vinput[0] == "exit")) {
			break;
		} else if (vinput[0] == "go") {
			// todo: currently ignoring but created variables for wtime, btime, winc, binc
			nodes = 0;
			start = (long)time(NULL); // seconds since 1/1/1970
            evaluating = true;
            m = movegen(search_depth);
            evaluating = false;
            makemove(m);
			end = (long)time(NULL);
			if (protocol == "local") { print_chessboard(); }
			int nps = 0;
			if ((end - start) > 0) {
				nps = nodes  / (long)(end - start);
			} else {
				nps = nodes;
			}
			cout << dec << "info string nps: " << nps << " nodes: " << nodes << " time: " << (end - start) << " seconds." << endl;
			print_bitboard("bb_black_rooks", bb_black_rooks);
            cout << "bestmove " << boardsquares[m.src] << boardsquares[m.dst] << endl;
		} else if (vinput[0] == "d") {
			print_chessboard();
        } else if (vinput[0] == "flip") {
            flip = !flip;
			if (protocol == "local") { print_chessboard(); }
        } else if (vinput[0] == "new") {
			new_game(fen_start_pos);
		} else if (vinput[0] == "ucinewgame") {
			new_game(fen_start_pos);
			isnewgame = true;
		} else if (vinput[0] == "fenload") {
			CFen cfenload = CFen();
			cout << "FEN Filename: ";
			cin >> fenfilename;
			new_game(cfenload.LoadFromFile(fenfilename));
		} else if (vinput[0] == "searchdepth") {
			cout << "Search Depth: ";
			cin >> search_depth;
		} else if (vinput[0] == "npstest") {
			search_depth = 5; // default using for benchmarking right now
			for (int x = 0; x < 10; x++) {
				nodes = 0;
#ifdef _WIN32
					start = GetTickCount();
#else
					start = (long)time(NULL); // seconds since 1/1/1970
#endif
				evaluating = true;
		        m = movegen(search_depth);
	            evaluating = false;
				makemove(m);
#ifdef _WIN32
					end = GetTickCount();
#else
					end = (long)time(NULL);
#endif

				if ((end - start) > 0) {
					nps = (nodes  / (end - start)) * 1000;
				} else {
					nps = nodes;
				}
	            cout << "info string [nps test] " << dec << "(" << x+1 << ") nps: " << nps << " nodes: " << nodes << " time: " << (end - start) << " ms." << endl;
			}
		} else if (vinput[0] == "uci") {
			cout << endl << "info string uci interface detected" << endl;
			cout << "id name SLiCE" << endl;
			cout << "id author Edwin Borowski" << endl;
			cout << "uciok" << endl;
			protocol = "uci";
		} else if (vinput[0] == "xboard") {
			cout << endl << "info string xboard protocol not currently supported" << endl;
		} else if (vinput[0] == "local") {
			cout << endl << "info string local interface detected" << endl;
			cout << "id name SLiCE" << endl;
			cout << "id author Edwin Borowski" << endl;
			cout << "localok" << endl;
			protocol = "local";
		} else if (vinput[0] == "isready") {
			cout << endl << "readyok" << endl;
		} else if (vinput[0] == "debug") {
			if (vinput.size() > 1) {
				if (vinput[1] == "on") {
					debug = 1;
				} else if (vinput[1] == "off") {
					debug = 0;
				} else {
					cout << "debug ";
					if (debug) { cout << "ON"; }
					else { cout << "OFF"; }
					cout << endl;
				}
			} else {
				cout << "debug ";
				if (debug) { cout << "ON"; }
				else { cout << "OFF"; }
				cout << endl;
			}
		} else if (vinput[0] == "print") {
			if (vinput.size() > 1) {
				// same as using "d"
				if (vinput[1] == "chesssboard") {
					print_chessboard();
                } else if (vinput[1] == "movelist") {
                    print_movelist();
				} else if (vinput[1] == "bitboards") {
					print_bitboard("bb_white_pawns", bb_white_pawns); 
					print_bitboard("bb_white_knights", bb_white_knights);
					print_bitboard("bb_white_bishops", bb_white_bishops);
					print_bitboard("bb_white_rooks", bb_white_rooks);
					print_bitboard("bb_white_queens", bb_white_queens);
					print_bitboard("bb_white_king", bb_white_king); 
					print_bitboard("bb_white_pieces", bb_white_pieces); 
					print_bitboard("bb_black_pawns", bb_black_pawns); 
					print_bitboard("bb_black_knights", bb_black_knights);
					print_bitboard("bb_black_bishops", bb_black_bishops);
					print_bitboard("bb_black_rooks", bb_black_rooks);
					print_bitboard("bb_black_queens", bb_black_queens);
					print_bitboard("bb_black_king", bb_black_king); 
					print_bitboard("bb_black_pieces", bb_black_pieces); 
					print_bitboard("bb_all_pieces", bb_all_pieces); 
					print_bitboard("bb_all_pieces90", bb_all_pieces90);
					print_bitboard("bb_all_pieces45l", bb_all_pieces45l);
					print_bitboard("bb_all_pieces45r", bb_all_pieces45r);
				}
			}
		} else if (vinput[0] == "register") {
			if (debug) { cout << endl << "info string [debug] registration not required." << endl; }
		} else if (vinput[0] == "eval") {
			int i = 0;
			if (vinput.size() > 1) {
				stringstream ss(vinput[1]);
				if ((ss >> i).fail()) { cout << "info string Invalid Parameter: " << vinput[1] << endl; }
			}
			if (i > 0) {
				evaluating = true;
			    m = movegen(i);
		        evaluating = false;
				cout << m.score << endl;
			} else {
				cout << eval() << endl;
			}
		} else if (vinput[0] == "position") {
			if (isnewgame) {
				if (vinput[1] == "startpos") {
					new_game(fen_start_pos);
				} else {
					new_game(vinput[1]); // todo: test this
				}
			}
			process_moves(vinput); // if isnewgame=true, we know to process entire movelist
			isnewgame = false;
		} else {
			cout << endl << "info string Invalid Command: " << input << endl;
		}
	}

    return 0;
}
