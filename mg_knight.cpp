/* mg_knight.cpp:
 * Generate and make all the knight moves
 */
#include "globals.h"
#include "proto.h"

int gen_knight_moves(move_t mv[]) {
	BITBOARD bb_knights_src = 0;
	BITBOARD bb_knight_dst = 0;
	BITBOARD bb_my_pieces = 0;
    move_t m;
	unsigned long src_idx = -1;
	unsigned long dst_idx = -1;
	int mv_idx = 0;

	if (white_to_move) {
		bb_knights_src = bb_white_knights;
		bb_my_pieces = bb_white_pieces;
		m.piece = WN;
	} else {
		bb_knights_src = bb_black_knights;
		bb_my_pieces = bb_black_pieces;
		m.piece = BN;
	}

	m.white_to_move = white_to_move;
    m.ply = ply;
    m.move_no = move_no;
	m.epsquare = epsquare;
	m.white_can_castle_kingside = white_can_castle_kingside;
	m.white_can_castle_queenside = white_can_castle_queenside;
	m.black_can_castle_kingside = black_can_castle_kingside;
	m.black_can_castle_queenside = black_can_castle_queenside;

	while (bb_knights_src > 0) {
		src_idx = bitScanForward(bb_knights_src);
		m.src = src_idx;
		bb_knights_src = bb_knights_src & (~bb_squares[src_idx]);
		bb_knight_dst = bb_legal_knight_moves[src_idx] & (~bb_my_pieces);
		while (bb_knight_dst > 0) {
			dst_idx = bitScanForward(bb_knight_dst);
			m.dst = dst_idx;
			m.capture = chessboard[dst_idx]; // is there a better way to handle if it's a capture?
			bb_knight_dst = bb_knight_dst & (~bb_squares[dst_idx]);
			mv[mv_idx++] = m;
		}
	}

	return mv_idx;
}

