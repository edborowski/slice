/* mg_pawn.cpp:
 * Generate and make all the pawn moves
 */
#include <iostream>
#include <vector>
#include "globals.h"
#include "proto.h"

/*
BITBOARD bbwhitepawnmoves[64] = {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
	0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x100000000, 0x200000000, 0x400000000, 0x800000000, 0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
	0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000, 0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
	0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000, 0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
	0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000,
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};
*/

int whitepawnmoves[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 63,
	-1, -1, -1, -1, -1, -1, -1, -1
};

int whitepawncapturesleft[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, 16, 17, 18, 19, 20, 21, 22,
	-1, 24, 25, 26, 27, 28, 29, 30,
	-1, 32, 33, 34, 35, 36, 37, 38,
	-1, 40, 41, 42, 43, 44, 45, 46,
	-1, 48, 49, 50, 51, 52, 53, 54,
	-1, 56, 57, 58, 59, 60, 61, 62,
	-1, -1, -1, -1, -1, -1, -1, -1
};

int whitepawncapturesright[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	17, 18, 19, 20, 21, 22, 23, -1,
	25, 26, 27, 28, 29, 30, 31, -1,
	33, 34, 35, 36, 37, 38, 39, -1,
	41, 42, 43, 44, 45, 46, 47, -1,
	49, 50, 51, 52, 53, 54, 55, -1,
	57, 58, 59, 60, 61, 62, 63, -1,
	-1, -1, -1, -1, -1, -1, -1, -1
};

int blackpawnmoves[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	 0,  1,  2,  3,  4,  5,  6,  7,
	 8,  9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 
	-1, -1, -1, -1, -1, -1, -1, -1
};

int blackpawncapturesleft[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	 1,  2,  3,  4,  5,  6,  7, -1,
	 9, 10, 11, 12, 13, 14, 15, -1, 
	17, 18, 19, 20, 21, 22, 23, -1,
	25, 26, 27, 28, 29, 30, 31, -1,
	33, 34, 35, 36, 37, 38, 39, -1,
	41, 42, 43, 44, 45, 46, 47, -1, 
	-1, -1, -1, -1, -1, -1, -1, -1
};

int blackpawncapturesright[64] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,
	-1,  8,  9, 10, 11, 12, 13, 14,
	-1, 16, 17, 18, 19, 20, 21, 22,
	-1, 24, 25, 26, 27, 28, 29, 30,
	-1, 32, 33, 34, 35, 36, 37, 38,
	-1, 40, 41, 42, 43, 44, 45, 46, 
	-1, -1, -1, -1, -1, -1, -1, -1
};

int gen_pawn_moves(move_t mv[]) {
	BITBOARD bb_pawns_src = 0;
	BITBOARD bb_pawns_dst = 0;
	BITBOARD bb_pawns_src_2 = 0; // move 2 squares ahead
	BITBOARD bb_pawns_dst_2 = 0; // move 2 squares ahead
	BITBOARD bb_pawns_cap7 = 0;
	BITBOARD bb_pawns_cap7_src = 0;
	BITBOARD bb_pawns_cap9 = 0;
	BITBOARD bb_pawns_cap9_src = 0;
    BITBOARD bb_epsquare = 0;
    move_t m;
	unsigned long src_idx = -1;
	unsigned long dst_idx = -1;
	unsigned long cap7_idx = -1;
	unsigned long cap9_idx = -1;
	int mv_idx = 0;
	int ep_pawn = 0;

    if (epsquare >= 0) { bb_epsquare = bb_squares[epsquare]; }
    
	if (white_to_move) {
		// the result will be we only have those pawns on the bitboard
		// where a legal move is possible
		bb_pawns_dst = (bb_white_pawns << 8);
		bb_pawns_cap7 = ((bb_pawns_dst >> 1) & (~bb_file[H_FILE])) & (bb_black_pieces | bb_epsquare);
		bb_pawns_cap9 = ((bb_pawns_dst << 1) & (~bb_file[A_FILE])) & (bb_black_pieces | bb_epsquare);
		// we have to ~not the black pieces after we calculate captures
		// otherwise a pawn that can't move forward will be erased from the board
		// even if it can capture
		bb_pawns_dst = bb_pawns_dst & (~bb_all_pieces);
		bb_pawns_dst_2 = ((bb_pawns_dst << 8) & (~bb_all_pieces)) & bb_rank[RANK_4];
		// we push back for the src squares so that we remove from the list any
		// pawns that can't move forward
		bb_pawns_src = bb_pawns_dst >> 8;
		bb_pawns_src_2 = bb_pawns_dst_2 >> 16;
		//todo: maybe work this out later
		bb_pawns_cap7_src = bb_pawns_cap7 >> 7;
		bb_pawns_cap9_src = bb_pawns_cap9 >> 9;
		m.piece = WP;
		ep_pawn = BP;
	} else {
		// the result will be we only have those pawns on the bitboard
		// where a legal move is possible
		bb_pawns_dst = (bb_black_pawns >> 8);
		bb_pawns_cap7 = ((bb_pawns_dst << 1) & (~bb_file[A_FILE])) & (bb_white_pieces | bb_epsquare);
		bb_pawns_cap9 = ((bb_pawns_dst >> 1) & (~bb_file[H_FILE])) & (bb_white_pieces | bb_epsquare);
		// we have to ~not the black pieces after we calculate captures
		// otherwise a pawn that can't move forward will be erased from the board
		// even if it can capture
		bb_pawns_dst = bb_pawns_dst & (~bb_all_pieces);
		bb_pawns_dst_2 = ((bb_pawns_dst >> 8) & (~bb_all_pieces)) & bb_rank[RANK_5];
		// we push back for the src squares so that we remove from the list any
		// pawns that can't move forward
		bb_pawns_src = bb_pawns_dst << 8;
		bb_pawns_src_2 = bb_pawns_dst_2 << 16;
		//todo: maybe work this out later
		bb_pawns_cap7_src = bb_pawns_cap7 << 7;
		bb_pawns_cap9_src = bb_pawns_cap9 << 9;
		m.piece = BP;
		ep_pawn = WP;
	}

	m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	m.white_can_castle_kingside = white_can_castle_kingside;
	m.white_can_castle_queenside = white_can_castle_queenside;
	m.black_can_castle_kingside = black_can_castle_kingside;
	m.black_can_castle_queenside = black_can_castle_queenside;

	// get move 1 ahead
	while (bb_pawns_src > 0) {
		src_idx = bitScanForward(bb_pawns_src); 
		m.src = src_idx;
		assert(src_idx < 64);
		dst_idx = bitScanForward(bb_pawns_dst);
		m.dst = dst_idx;
		bb_pawns_src = bb_pawns_src & (~bb_squares[src_idx]);
		bb_pawns_dst = bb_pawns_dst & (~bb_squares[dst_idx]);
		m.capture = 0;
		mv[mv_idx++] = m;
	}
	assert(bb_pawns_src == 0);
	assert(bb_pawns_dst == 0);

	// get move 2 ahead
	while (bb_pawns_src_2 > 0) {
		src_idx = bitScanForward(bb_pawns_src_2); 
		m.src = src_idx;
		dst_idx = bitScanForward(bb_pawns_dst_2);
		m.dst = dst_idx;
		bb_pawns_src_2 = bb_pawns_src_2 & (~bb_squares[src_idx]);
		bb_pawns_dst_2 = bb_pawns_dst_2 & (~bb_squares[dst_idx]);
		m.capture = 0;
		mv[mv_idx++] = m;
	}
	assert(bb_pawns_src_2 == 0);
	assert(bb_pawns_dst_2 == 0);

	// get move captures left
	while (bb_pawns_cap7 > 0) {
		src_idx = bitScanForward(bb_pawns_cap7_src);
		cap7_idx = bitScanForward(bb_pawns_cap7);
		m.src = src_idx;
		m.dst = cap7_idx;
        //cout << "TOMOVE: " << white_to_move << " -> " << move_to_long_algebraic(m) << " epsquare:" << epsquare << endl;
        //print_bitboard("bb_white_pieces", bb_white_pieces);
        //print_bitboard("bb_epsquare", bb_epsquare);        
        //print_bitboard("bb_pawns_cap7_src", bb_pawns_cap7_src);
        //print_bitboard("bb_pawns_cap7", bb_pawns_cap7);
		bb_pawns_cap7_src = bb_pawns_cap7_src & (~bb_squares[src_idx]);
		bb_pawns_cap7 = bb_pawns_cap7 & (~bb_squares[cap7_idx]);
		if (cap7_idx == epsquare) {
			m.capture = ep_pawn;
		} else {
			m.capture = chessboard[cap7_idx];
		}
		mv[mv_idx++] = m;
	}
	assert(bb_pawns_cap7 == 0);
	assert(bb_pawns_cap7_src == 0);
    
	// get move captures right
	while (bb_pawns_cap9 > 0) {
		src_idx = bitScanForward(bb_pawns_cap9_src);
		cap9_idx = bitScanForward(bb_pawns_cap9);
		m.src = src_idx;
		m.dst = cap9_idx;
		bb_pawns_cap9_src = bb_pawns_cap9_src & (~bb_squares[src_idx]);
		bb_pawns_cap9 = bb_pawns_cap9 & (~bb_squares[cap9_idx]);
		if (cap9_idx == epsquare) {
			m.capture = ep_pawn;
		} else {
			m.capture = chessboard[cap9_idx];
		}
		mv[mv_idx++] = m;
	}
	assert(bb_pawns_cap9 == 0);
	assert(bb_pawns_cap9_src == 0);
    
	return mv_idx;
}

/* pawngenw():
 * Generate and make all the legal white pawn moves
 */
int pawngenw(int square, move_t mv[]) {
    move_t m;
	int mv_idx = 0;

    m.src = square;
    m.piece = 1;
    m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	
    // is dst square empty 1 square ahead
    if (chessboard[whitepawnmoves[square]] == 0) {
        m.capture = 0;
        m.dst = whitepawnmoves[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }

    // can we move 2 squares on pawns first move?
    if ((chessboard[whitepawnmoves[square]] == 0) && (chessboard[square+16] == 0) && (square < 16)) {
        m.capture = 0;
        m.dst = square+16;
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }

    // can we capture left
    if (chessboard[whitepawncapturesleft[square]] == -1) {
        m.capture = -1;
        m.dst = whitepawncapturesleft[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }

    // can we capture right
    if (chessboard[whitepawncapturesright[square]] == -1) {
        m.capture = -1;
        m.dst = whitepawncapturesright[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }
	
	if (epsquare > 0) {
		// can we capture enpassant left
	    if (whitepawncapturesleft[square] == epsquare) {
			m.capture = -1;
		    m.dst = whitepawncapturesleft[square];
			assert(m.dst > -1);
			mv[mv_idx++] = m;
		}
	
		// can we capture enpassant right
		if (whitepawncapturesright[square] == epsquare) {
	        m.capture = -1;
		    m.dst = whitepawncapturesright[square];
			assert(m.dst > -1);
			mv[mv_idx++] = m;
	    }
	}

    return mv_idx;
}

/* pawngenw():
 * Generate and make all the legal white pawn moves
 */
//vector<move_t> pawngenw(int square) {
int pawngenw_old(int square, move_t mv[]) {
    //vector<move_t> mv;
	//move_t mv[MAX_MOVES];
    move_t m;
	int mv_idx = 0;

    m.src = square;
    m.piece = 1;
    m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	
    // is dst square empty 1 square ahead
    if (chessboard[square+8] == 0) {
        m.capture = 0;
        m.dst = square+8;
	    //mv.push_back(m);
		mv[mv_idx++] = m;
    } //else {
        //m.dst = -1; // no valid move
    //}
    //mv.push_back(m);

    // can we move 2 squares on pawns first move?
    if ((chessboard[square+8] == 0) && (chessboard[square+16] == 0) && (square < 16)) {
        m.capture = 0;
        m.dst = square+16;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }

    // can we capture left
    if ((chessboard[square+7] == -1) && (square != 0 && square != 8 && square != 16 && square != 24 && square != 32 && square != 40 && square != 48 && square != 56))  {
        m.capture = -1;
        m.dst = square+7;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }

    // can we capture right
    if ((chessboard[square+9] == -1) && (square != 7 && square != 15 && square != 23 && square != 31 && square != 39 && square != 47 && square != 55 && square != 63))  {
        m.capture = -1;
        m.dst = square+9;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }
	
	// can we capture enpassant left
    if ((square+7 == epsquare) && (square != 0 && square != 8 && square != 16 && square != 24 && square != 32 && square != 40 && square != 48 && square != 56))  {
        m.capture = -1;
        m.dst = square+7;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }
	
	// can we capture enpassant right
    if ((square+9 == epsquare) && (square != 7 && square != 15 && square != 23 && square != 31 && square != 39 && square != 47 && square != 55 && square != 63))  {
        m.capture = -1;
        m.dst = square+9;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }

    return mv_idx;
}

/* pawngenb():
 * Generate and make all the legal black pawn moves
 */
//vector<move_t> pawngenb(int square) {
int pawngenb(int square, move_t mv[]) {
    move_t m;
	int mv_idx = 0;

    m.src = square;
    m.piece = -1;
    m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	
    // is dst square empty
    if (chessboard[blackpawnmoves[square]] == 0) {
        m.capture = 0;
        m.dst = blackpawnmoves[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }

    // can we move 2 squares on pawns first move?
    if ((chessboard[blackpawnmoves[square]] == 0) && (chessboard[square-16] == 0) && (square > 47)) {
        m.capture = 0;
        m.dst = square-16;
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }

    // can we capture left
    if (chessboard[blackpawncapturesleft[square]] == 1) {
        m.capture = 1;
        m.dst = blackpawncapturesleft[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }
	
    // can we capture right
    if (chessboard[blackpawncapturesright[square]] == 1) {
        m.capture = 1;
        m.dst = blackpawncapturesright[square];
		assert(m.dst > -1);
		mv[mv_idx++] = m;
    }
	
	if (epsquare > 0) {
		// can we capture enpassant left
		if (blackpawncapturesleft[square] == epsquare) {
			m.capture = 1;
			m.dst = blackpawncapturesleft[square];
			assert(m.dst > -1);
			mv[mv_idx++] = m;
		}
	
		// can we capture enpassant right
		if (blackpawncapturesright[square] == epsquare) {
			m.capture = 1;
		    m.dst = blackpawncapturesright[square];
			assert(m.dst > -1);
			mv[mv_idx++] = m;
	    }
	}

    return mv_idx;
}

/* pawngenb():
 * Generate and make all the legal black pawn moves
 */
//vector<move_t> pawngenb(int square) {
int pawngenb_old(int square, move_t mv[]) {
    //vector<move_t> mv;
	//move_t mv[MAX_MOVES];
    move_t m;
	int mv_idx = 0;

    m.src = square;
    m.piece = -1;
    m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	
    // is dst square empty
    if (chessboard[square-8] == 0) {
        m.capture = 0;
        m.dst = square-8;
	    //mv.push_back(m);
		mv[mv_idx++] = m;
    } //else {
        //m.dst = -1; // no valid move
    //}
    //mv.push_back(m);

    // can we move 2 squares on pawns first move?
    if ((chessboard[square-8] == 0) && (chessboard[square-16] == 0) && (square > 47)) {
        m.capture = 0;
        m.dst = square-16;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }

    // can we capture left
    if ((chessboard[square-7] == 1) && (square != 7 && square != 15 && square != 23 && square != 31 && square != 39 && square != 47 && square != 55 && square != 63))  {
        m.capture = 1;
        m.dst = square-7;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }
	
    // can we capture right
    if ((chessboard[square-9] == 1) && (square != 0 && square != 8 && square != 16 && square != 24 && square != 32 && square != 40 && square != 48 && square != 56))  {
        m.capture = 1;
        m.dst = square-9;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }
	
	// can we capture enpassant left
    if ((square-7 == epsquare) && (square != 7 && square != 15 && square != 23 && square != 31 && square != 39 && square != 47 && square != 55 && square != 63))  {
        m.capture = 1;
        m.dst = square-7;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }
	
	// can we capture enpassant right
    if ((square-9 == epsquare) && (square != 0 && square != 8 && square != 16 && square != 24 && square != 32 && square != 40 && square != 48 && square != 56))  {
        m.capture = 1;
        m.dst = square-9;
        //mv.push_back(m);
		mv[mv_idx++] = m;
    }

    return mv_idx;
}
