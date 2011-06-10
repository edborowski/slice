/* mg_bishop.cpp
 * Generate and make all the bishop moves
 */
#include "globals.h"
#include "proto.h"

int gen_bishop_moves(move_t mv[]) {
	BITBOARD bb_bishops_src = 0;
	BITBOARD bb_bishop_dst = 0;
	BITBOARD bb_my_pieces = 0;
	BITBOARD bb_idx45l = 0;
	BITBOARD bb_idx45r = 0;

    move_t m;
	unsigned long src_idx = -1;
	unsigned long src_idx45l = -1;
	unsigned long src_idx45r = -1;
	unsigned long dst_idx = -1;
	int mv_idx = 0;

	if (white_to_move) {
		bb_bishops_src = bb_white_bishops | bb_white_queens;
		bb_my_pieces = bb_white_pieces;
	} else {
		bb_bishops_src = bb_black_bishops | bb_black_queens;
		bb_my_pieces = bb_black_pieces;
	}

	m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	m.white_can_castle_kingside = white_can_castle_kingside;
	m.white_can_castle_queenside = white_can_castle_queenside;
	m.black_can_castle_kingside = black_can_castle_kingside;
	m.black_can_castle_queenside = black_can_castle_queenside;

	while (bb_bishops_src > 0) {
		src_idx = bitScanForward(bb_bishops_src);
		src_idx45l = rotate45l[src_idx];
		src_idx45r = rotate45r[src_idx];
		m.src = src_idx;
		m.piece = chessboard[src_idx];
		bb_bishops_src = bb_bishops_src & (~bb_squares[src_idx]);
        bb_idx45l = (bb_all_pieces45l >> shift45[src_idx45l]) & (BITBOARD)mask45[src_idx45l];
        bb_idx45r = (bb_all_pieces45r >> shift45[src_idx45r]) & (BITBOARD)mask45[src_idx45r];
        bb_bishop_dst = (bb_legal_sliding45l_moves[src_idx][(int)bb_idx45l] | bb_legal_sliding45r_moves[src_idx][(int)bb_idx45r]) & (~bb_my_pieces);
		while (bb_bishop_dst > 0) {
			dst_idx = bitScanForward(bb_bishop_dst);
			m.dst = dst_idx;
			m.capture = chessboard[dst_idx]; // is there a better way to handle if it's a capture?
			bb_bishop_dst = bb_bishop_dst & (~bb_squares[dst_idx]);
			mv[mv_idx++] = m;
		}
	}

	return mv_idx;
}
