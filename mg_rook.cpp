/* mg_rook.cpp
 * Generate and make all the rook moves
 */
#include "globals.h"
#include "proto.h"

int gen_rook_moves(move_t mv[]) {
	BITBOARD bb_rooks_src = 0;
	BITBOARD bb_rook_dst = 0;
	BITBOARD bb_my_pieces = 0;
	BITBOARD bb_idx0 = 0;
	BITBOARD bb_idx90 = 0;

    move_t m;
	unsigned long src_idx = -1;
	unsigned long src_idx90 = -1;
	unsigned long dst_idx = -1;
	int mv_idx = 0;

	if (white_to_move) {
		bb_rooks_src = bb_white_rooks | bb_white_queens;
		bb_my_pieces = bb_white_pieces;
	} else {
		bb_rooks_src = bb_black_rooks | bb_black_queens;
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

	while (bb_rooks_src > 0) {
		src_idx = bitScanForward(bb_rooks_src);
		src_idx90 = rotate90[src_idx];
		m.src = src_idx;
		m.piece = chessboard[src_idx];
		bb_rooks_src = bb_rooks_src & (~bb_squares[src_idx]);
        //shift right/left to get rid of all but the row
		// we need to do all pieces instead of just opposite color because
		// there could be a same color piece that impacts the movement
        //TODO: instead of shifting twice, shift once then AND with 255.
        bb_idx0 = bb_all_pieces << ((7 - (src_idx / 8)) * 8) >> 56;
        bb_idx90 = bb_all_pieces90 << ((7 - (src_idx90 / 8)) * 8) >> 56;
        //bbRookMoves = (bbLegalSliding0Moves[index, (int)idx] | bbLegalSliding90Moves[index, (int)idx90]) & (~pos.bbWhitePieces);
		bb_rook_dst = bb_legal_slidingh_moves[src_idx][bb_idx0] & (~bb_my_pieces);
		while (bb_rook_dst > 0) {
			dst_idx = bitScanForward(bb_rook_dst);
			m.dst = dst_idx;
			m.capture = chessboard[dst_idx]; // is there a better way to handle if it's a capture?
			bb_rook_dst = bb_rook_dst & (~bb_squares[dst_idx]);
			mv[mv_idx++] = m;
		}
	}

	return mv_idx;
}

