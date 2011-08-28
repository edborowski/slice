#include <cmath>
#include <cstdlib>
#include "globals.h"
#include "proto.h"

void gen_static_squares_bitboards(void) {
	int i = 0;
	BITBOARD bb = 1;

	for (i = 0; i < 64; i++) {
		bb_squares[i] = bb << i;
	}
}

void gen_static_king_bitboards(void) {
	int i = 0;
	// code taken from adept (c#) by Ed Borowski
	//Start with a king on a square with a full range of motion (B2)
    bb_legal_king_moves[9] = 0x0000000000070507;

    //shift it around the board to create for all squares
    for (i = 0; i < 64; i++) {
        if (i == 07) {
            bb_legal_king_moves[i] = (bb_legal_king_moves[9] >> (9 - i)) & ~bb_file[A_FILE];
        }
        else if (i < 9) {
            bb_legal_king_moves[i] = bb_legal_king_moves[9] >> (9 - i);
        }
        else if (i > 9) {
            bb_legal_king_moves[i] = bb_legal_king_moves[9] << (i - 9);
        }

        //if it's on the edge of the board, we need to mask the opposite file
        if ((i % 8) == 0) {
            bb_legal_king_moves[i] = bb_legal_king_moves[i] & ~bb_file[H_FILE];
        }
        else if (((i % 8) == 7)) {
            bb_legal_king_moves[i] = bb_legal_king_moves[i] & ~bb_file[A_FILE];
        }
    }
}

void gen_static_knight_bitboards(void) {
	int possibleMoves[8];
    BITBOARD bitmask, bit;

    for(int i = 0; i < 64; i++) {
		bitmask = 0;
        possibleMoves[0] = i + 10;
        possibleMoves[1] = i +  6;
        possibleMoves[2] = i + 17;
        possibleMoves[3] = i + 15;
        possibleMoves[4] = i - 10;
        possibleMoves[5] = i -  6;
        possibleMoves[6] = i - 17;
        possibleMoves[7] = i - 15;

        for(int j = 0; j < 8; j++) {
			if ( possibleMoves[j] < 0 || possibleMoves[j] > 63 ) {
				continue;
            }

            switch(abs(possibleMoves[j]/8 - i/8)) {
				case 1: if ( abs(i%8 - possibleMoves[j]%8) != 2 ) { continue; } break;
                case 2: if ( abs(i%8 - possibleMoves[j]%8) != 1 ) { continue; } break;
                default:    continue;
            }
        
            bit = 1;
            bit <<= possibleMoves[j];
            bitmask |= bit;
        }

        bb_legal_knight_moves[i] = bitmask;
    }
}

void gen_static_slidingh_bitboards(void) {
    BITBOARD bb_pieces;
    int piece_pos = 0;
    BITBOARD bb_current_square = 0;
    BITBOARD bitmask = 1;

    for (int rank = 0; rank < 8; rank++) {
	    for (int file = 0; file < 8; file++) {
		    piece_pos = rank * 8 + file;
            for (int pos = 0; pos < 256; pos++) {
			    bb_pieces = (BITBOARD)pos;
                BITBOARD bb_slidingh_moves = 0;
                for (int pos2 = file + 1; pos2 < 8; pos2++) {
				    bb_current_square = bitmask << pos2;
                    bb_slidingh_moves = bb_slidingh_moves | bb_current_square;
                    if ((bb_pieces & bb_current_square) > 0) {
					    break;
                    }
                }
                int endsq = file - 1;
                for (int pos2 = endsq; pos2 >= 0; pos2--) {
					bb_current_square = bitmask << pos2;
                    bb_slidingh_moves = bb_slidingh_moves | bb_current_square;
                    if ((bb_pieces & bb_current_square) > 0) {
						break;
                    }
                }                        
                bb_legal_slidingh_moves[piece_pos][pos] = bb_slidingh_moves << rank * 8;
           }
        }
    }
}

void gen_static_slidingv_bitboards(void) {
	BITBOARD bitmask = 1;
	int square90 = 0;

    // To create our bitboards for sliding on the file, we will
    // use the lookup table (arrayRotate90) to determine the appropriate
    // rank bitboard to rotate, which will create our rotated bitboard
    for (int square = 0; square < 64; square++) {
        square90 = rotate90[square];

        for (int idx = 0; idx < 256; idx++) {
            //rotate
            for (int ridx = 0; ridx < 64; ridx++) {
                if ((bb_legal_slidingh_moves[square90][idx] & (bitmask << ridx)) > 0) {
                    bb_legal_slidingv_moves[square][idx] = bb_legal_slidingv_moves[square][idx] | (bitmask << rotate90[ridx]);
                }
            }
        }
    }
}

void gen_static_sliding45l_bitboards(void) {
	int starting_squarel[] = { 0, 1, 2, 3, 4, 5, 6, 7,
                               1, 2, 3, 4, 5, 6, 7,15,
                               2, 3, 4, 5, 6, 7,15,23,
                               3, 4, 5, 6, 7,15,23,31,
                               4, 5, 6, 7,15,23,31,39,
                               5, 6, 7,15,23,31,39,47,
                               6, 7,15,23,31,39,47,55,
                               7,15,23,31,39,47,55,63 };
	int lengthl[] = { 1, 2, 3, 4, 5, 6, 7, 8,
                      2, 3, 4, 5, 6, 7, 8, 7,
                      3, 4, 5, 6, 7, 8, 7, 6,
                      4, 5, 6, 7, 8, 7, 6, 5,
                      5, 6, 7, 8, 7, 6, 5, 4,
                      6, 7, 8, 7, 6, 5, 4, 3,
                      7, 8, 7, 6, 5, 4, 3, 2,
                      8, 7, 6, 5, 4, 3, 2, 1 };
	int positionl[] = { 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 1, 1, 1, 1, 1, 1, 0,
                        2, 2, 2, 2, 2, 2, 1, 0,
                        3, 3, 3, 3, 3, 2, 1, 0,
                        4, 4, 4, 4, 3, 2, 1, 0,
                        5, 5, 5, 4, 3, 2, 1, 0,
                        6, 6, 5, 4, 3, 2, 1, 0, 
                        7, 6, 5, 4, 3, 2, 1, 0 };
    BITBOARD bitmask = 1;

    for (int sq = 0; sq < 64; sq++) {
        // find the starting square for the ray we're sliding on
        int start_sq = starting_squarel[sq];

		// find the position in the ray we're sliding on
        int start_pos = positionl[sq];

        // initialize the array before we use it since we won't always
        // be filling it completely
        for (int idx = 0; idx < 256; idx++) {
            bb_legal_sliding45l_moves[sq][idx] = 0;
        }

        //fill the array
        int maxval = (int)pow(2.0, lengthl[sq]); // had to change 2 to 2.0 
        BITBOARD bb_attackl_board = 0;
        // we don't need to go all the way to 255 (11111111) since most rays won't be
        // 8 squares long
        for (int idx = 0; idx < maxval; idx++) {
            bb_attackl_board = bb_legal_slidingh_moves[start_pos][idx]; // todo: test weird using slidingh
            int last_sq = start_sq;
            // we only need to add the first x bits to our array
            for (int ridx = 0; ridx < lengthl[sq]; ridx++) {
                BITBOARD bb_bit_to_move = (bb_attackl_board >> ridx) & bitmask;
                bb_legal_sliding45l_moves[sq][idx] = bb_legal_sliding45l_moves[sq][idx] | (bb_bit_to_move << last_sq);
                last_sq = last_sq + 7;
            }
        }
    }
}

void gen_static_sliding45r_bitboards(void) {
	int starting_squarer[] = { 0, 1, 2, 3, 4, 5, 6, 7,
                               8, 0, 1, 2, 3, 4, 5, 6,
                              16, 8, 0, 1, 2, 3, 4, 5,
                              24,16, 8, 0, 1, 2, 3, 4,
                              32,24,16, 8, 0, 1, 2, 3,
                              40,32,24,16, 8, 0, 1, 2,
                              48,40,32,24,16, 8, 0, 1,
                              56,48,40,32,24,16, 8, 0 };
	int lengthr[] = { 8, 7, 6, 5, 4, 3, 2, 1,
                      7, 8, 7, 6, 5, 4, 3, 2,
                      6, 7, 8, 7, 6, 5, 4, 3,
                      5, 6, 7, 8, 7, 6, 5, 4,
                      4, 5, 6, 7, 8, 7, 6, 5,
                      3, 4, 5, 6, 7, 8, 7, 6,
                      2, 3, 4, 5, 6, 7, 8, 7,
                      1, 2, 3, 4, 5, 6, 7, 8 };            
	int positionr[] = { 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 1, 1, 1, 1, 1, 1, 1,
                        0, 1, 2, 2, 2, 2, 2, 2,
                        0, 1, 2, 3, 3, 3, 3, 3,
                        0, 1, 2, 3, 4, 4, 4, 4,
                        0, 1, 2, 3, 4, 5, 5, 5,
                        0, 1, 2, 3, 4, 5, 6, 6, 
                        0, 1, 2, 3, 4, 5, 6, 7 };
    BITBOARD bitmask = 1;

    for (int sq = 0; sq < 64; sq++) {
        // find the starting square for the ray we're sliding on
        int start_sq = starting_squarer[sq];

        // find the position in the ray we're sliding on
        int start_pos = positionr[sq];

        // initialize the array before we use it since we won't always
        // be filling it completely
        for (int idx = 0; idx < 256; idx++) {
            bb_legal_sliding45r_moves[sq][idx] = 0;
        }

        //fill the array
        int maxval = (int)pow(2.0, lengthr[sq]);
        BITBOARD bb_attackr_board = 0;
        // we don't need to go all the way to 255 (11111111) since most rays won't be
        // 8 squares long
        for (int idx = 0; idx < maxval; idx++) {
            bb_attackr_board = bb_legal_slidingh_moves[start_pos][idx];
            int last_sq = start_sq;
            // we only need to add the first x bits to our array
            for (int ridx = 0; ridx < lengthr[sq]; ridx++) {
                BITBOARD bb_bit_to_move = (bb_attackr_board >> ridx) & bitmask;
                bb_legal_sliding45r_moves[sq][idx] = bb_legal_sliding45r_moves[sq][idx] | (bb_bit_to_move << last_sq);
                last_sq = last_sq + 9;
            }
        }
    }
}

