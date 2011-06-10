/* mg_king.cpp:
 * Generate and make all the king moves
 */
#include "globals.h"
#include "proto.h"

int gen_king_moves(move_t mv[]) {
	BITBOARD bb_king_dst = 0;
	BITBOARD bb_castle_kingside = 0;
	BITBOARD bb_castle_queenside = 0;
	bool can_castle_kingside = true;
	bool can_castle_queenside = true;
	int dst_castle_kingside = 0;
	int dst_castle_queenside = 0;
    move_t m;
	unsigned long src_idx = -1;
	unsigned long dst_idx = -1;
	int mv_idx = 0;

	// There are no legal king moves to make if the king
	// isn't on the board
	//assert((bb_white_king > 0) && (bb_black_king > 0));
	if ((bb_white_king == 0) || (bb_black_king == 0)) {
		return 0;
	}

	if (white_to_move) {
		// get the square that the king is on
		src_idx = bitScanForward(bb_white_king); 
		// for moves, any square that's not a white piece
		bb_king_dst = bb_legal_king_moves[src_idx] & (~bb_white_pieces);
		bb_castle_kingside = bb_white_castle_kingside;
		bb_castle_queenside = bb_white_castle_queenside;
		can_castle_kingside = white_can_castle_kingside;
		can_castle_queenside = white_can_castle_queenside;
		dst_castle_kingside = DST_WHITE_KINGSIDE;
		dst_castle_queenside = DST_WHITE_QUEENSIDE;
		m.piece = WK;
	} else {
		// get the square that the king is on
		src_idx = bitScanForward(bb_black_king); 
		// for moves, any square that's not a black piece
		bb_king_dst = bb_legal_king_moves[src_idx] & (~bb_black_pieces);
		bb_castle_kingside = bb_black_castle_kingside;
		bb_castle_queenside = bb_black_castle_queenside;
		can_castle_kingside = black_can_castle_kingside;
		can_castle_queenside = black_can_castle_queenside;
		dst_castle_kingside = DST_BLACK_KINGSIDE;
		dst_castle_queenside = DST_BLACK_QUEENSIDE;
		m.piece = BK;
	}

	m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	m.src = src_idx;
	m.white_can_castle_kingside = white_can_castle_kingside;
	m.white_can_castle_queenside = white_can_castle_queenside;
	m.black_can_castle_kingside = black_can_castle_kingside;
	m.black_can_castle_queenside = black_can_castle_queenside;

	while (bb_king_dst > 0) {
		//m.src = src_idx;
		dst_idx = bitScanForward(bb_king_dst);
		m.dst = dst_idx;
		m.capture = chessboard[dst_idx]; // is there a better way to handle if it's a capture?
		bb_king_dst = bb_king_dst & (~bb_squares[dst_idx]);
		mv[mv_idx++] = m;
	}
	if (can_castle_kingside) {
		if (!(bb_castle_kingside & bb_all_pieces)) {
			m.dst = dst_castle_kingside;
			m.capture = 0;
			mv[mv_idx++] = m;
		}
	}
	if (can_castle_queenside) {
		if (!(bb_castle_queenside & bb_all_pieces)) {
			m.dst = dst_castle_queenside;
			m.capture = 0;
			mv[mv_idx++] = m;
		}
	}
    
    if (white_to_move) {
        white_can_castle_kingside = false;
        white_can_castle_queenside = false;
    } else {
        black_can_castle_kingside = false;
        black_can_castle_queenside = false;
    }

	return mv_idx;
}

