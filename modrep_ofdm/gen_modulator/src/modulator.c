/* Modulation Tables and Functions */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "modulator.h"

#define INCLUDE_DEFS_ONLY
#include "gen_modulator.h"

/* Static memory allocation for Modulation Tables for all modulation types */
output_t bpsk_table[2];
output_t qpsk_table[4];
output_t qam16_table[16];
output_t qam64_table[64];


/**
 * Set the BPSK modulation table */
inline void set_BPSKtable(void)
{
	// LTE-BPSK constellation:
	//    Q
	//    |  0
	//---------> I
	// 1  |
	bpsk_table[0] = BPSK_LEVEL + BPSK_LEVEL*_Complex_I;
	bpsk_table[1] = -BPSK_LEVEL -BPSK_LEVEL*_Complex_I;
}

/**
 * Set the QPSK modulation table */
inline void set_QPSKtable(void)
{
	// LTE-QPSK constellation:
	//     Q
	// 10  |  00
	//-----------> I
	// 11  |  01
	qpsk_table[0] = QPSK_LEVEL + QPSK_LEVEL*_Complex_I;
	qpsk_table[1] = QPSK_LEVEL - QPSK_LEVEL*_Complex_I;
	qpsk_table[2] = -QPSK_LEVEL + QPSK_LEVEL*_Complex_I;
	qpsk_table[3] = -QPSK_LEVEL - QPSK_LEVEL*_Complex_I;
}

/**
 * Set the 16QAM modulation table */
inline void set_16QAMtable(void)
{
	// LTE-16QAM constellation:
	//                Q
	//  1011	1001  |	 0001	0011
	//  1010	1000  |	 0000	0010
	//---------------------------------> I
	//  1110    1100  |  0100	0110
	//  1111    1101  |  0101	0111
	qam16_table[0] = QAM16_LEVEL_1 + QAM16_LEVEL_1*_Complex_I;
	qam16_table[1] = QAM16_LEVEL_1 + QAM16_LEVEL_2*_Complex_I;
	qam16_table[2] = QAM16_LEVEL_2 + QAM16_LEVEL_1*_Complex_I;
	qam16_table[3] = QAM16_LEVEL_2 + QAM16_LEVEL_2*_Complex_I;
	qam16_table[4] = QAM16_LEVEL_1 - QAM16_LEVEL_1*_Complex_I;
	qam16_table[5] = QAM16_LEVEL_1 - QAM16_LEVEL_2*_Complex_I;
	qam16_table[6] = QAM16_LEVEL_2 - QAM16_LEVEL_1*_Complex_I;
	qam16_table[7] = QAM16_LEVEL_2 - QAM16_LEVEL_2*_Complex_I;
	qam16_table[8] = -QAM16_LEVEL_1 + QAM16_LEVEL_1*_Complex_I;
	qam16_table[9] = -QAM16_LEVEL_1 + QAM16_LEVEL_2*_Complex_I;
	qam16_table[10] = -QAM16_LEVEL_2 + QAM16_LEVEL_1*_Complex_I;
	qam16_table[11] = -QAM16_LEVEL_2 + QAM16_LEVEL_2*_Complex_I;
	qam16_table[12] = -QAM16_LEVEL_1 - QAM16_LEVEL_1*_Complex_I;
	qam16_table[13] = -QAM16_LEVEL_1 - QAM16_LEVEL_2*_Complex_I;
	qam16_table[14] = -QAM16_LEVEL_2 - QAM16_LEVEL_1*_Complex_I;
	qam16_table[15] = -QAM16_LEVEL_2 - QAM16_LEVEL_2*_Complex_I;
}

/**
 * Set the 64QAM modulation table */
inline void set_64QAMtable(void)
{
	// LTE-64QAM constellation:
	// see [3GPP TS 36.211 version 10.5.0 Release 10, Section 7.1.4]
	qam64_table[0] = QAM64_LEVEL_2 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[1] = QAM64_LEVEL_2 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[2] = QAM64_LEVEL_1 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[3] = QAM64_LEVEL_1 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[4] = QAM64_LEVEL_2 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[5] = QAM64_LEVEL_2 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[6] = QAM64_LEVEL_1 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[7] = QAM64_LEVEL_1 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[8] = QAM64_LEVEL_3 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[9] = QAM64_LEVEL_3 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[10] = QAM64_LEVEL_4 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[11] = QAM64_LEVEL_4 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[12] = QAM64_LEVEL_3 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[13] = QAM64_LEVEL_3 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[14] = QAM64_LEVEL_4 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[15] = QAM64_LEVEL_4 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[16] = QAM64_LEVEL_2 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[17] = QAM64_LEVEL_2 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[18] = QAM64_LEVEL_1 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[19] = QAM64_LEVEL_1 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[20] = QAM64_LEVEL_2 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[21] = QAM64_LEVEL_2 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[22] = QAM64_LEVEL_1 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[23] = QAM64_LEVEL_1 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[24] = QAM64_LEVEL_3 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[25] = QAM64_LEVEL_3 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[26] = QAM64_LEVEL_4 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[27] = QAM64_LEVEL_4 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[28] = QAM64_LEVEL_3 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[29] = QAM64_LEVEL_3 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[30] = QAM64_LEVEL_4 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[31] = QAM64_LEVEL_4 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[32] = -QAM64_LEVEL_2 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[33] = -QAM64_LEVEL_2 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[34] = -QAM64_LEVEL_1 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[35] = -QAM64_LEVEL_1 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[36] = -QAM64_LEVEL_2 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[37] = -QAM64_LEVEL_2 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[38] = -QAM64_LEVEL_1 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[39] = -QAM64_LEVEL_1 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[40] = -QAM64_LEVEL_3 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[41] = -QAM64_LEVEL_3 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[42] = -QAM64_LEVEL_4 + QAM64_LEVEL_2*_Complex_I;
	qam64_table[43] = -QAM64_LEVEL_4 + QAM64_LEVEL_1*_Complex_I;
	qam64_table[44] = -QAM64_LEVEL_3 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[45] = -QAM64_LEVEL_3 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[46] = -QAM64_LEVEL_4 + QAM64_LEVEL_3*_Complex_I;
	qam64_table[47] = -QAM64_LEVEL_4 + QAM64_LEVEL_4*_Complex_I;
	qam64_table[48] = -QAM64_LEVEL_2 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[49] = -QAM64_LEVEL_2 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[50] = -QAM64_LEVEL_1 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[51] = -QAM64_LEVEL_1 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[52] = -QAM64_LEVEL_2 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[53] = -QAM64_LEVEL_2 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[54] = -QAM64_LEVEL_1 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[55] = -QAM64_LEVEL_1 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[56] = -QAM64_LEVEL_3 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[57] = -QAM64_LEVEL_3 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[58] = -QAM64_LEVEL_4 - QAM64_LEVEL_2*_Complex_I;
	qam64_table[59] = -QAM64_LEVEL_4 - QAM64_LEVEL_1*_Complex_I;
	qam64_table[60] = -QAM64_LEVEL_3 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[61] = -QAM64_LEVEL_3 - QAM64_LEVEL_4*_Complex_I;
	qam64_table[62] = -QAM64_LEVEL_4 - QAM64_LEVEL_3*_Complex_I;
	qam64_table[63] = -QAM64_LEVEL_4 - QAM64_LEVEL_4*_Complex_I;
}

/**
 * BPSK modulation function
 * Converts a single bit to a complex symbol
 * @param bits pointer to input bit
 * @param S_out pointer to complex symbol */
inline void modulate_BPSK(char *bits, output_t *S_out)
{
	int k;
//	if ((symbol_in > 1) || (symbol_in < 0))
//		printf("ERROR: Unknown input symbol for BPSK: %d\n",symbol_in);
//	else {
//		*I_out = __real__ bpsk_table[symbol_in];
//		*Q_out = __imag__ bpsk_table[symbol_in];
//	}
	k=(bits[0] == 1) ? 1 : 0;
	*S_out = bpsk_table[k];
}

/**
 * QPSK modulation function
 * Converts bitsequence of 2 bits to complex symbol
 * @param bits pointer to input bit sequence
 * @param S_out pointer to complex symbol */
inline void modulate_QPSK(char *bits, output_t *S_out)
{
	int k;
	k = ((bits[0] == 1) ? 1 : 0) <<1 | ((bits[1] == 1) ? 1 : 0);
	*S_out = qpsk_table[k];
}

/**
 * 16QAM modulation function
 * Converts bitsequence of 4 bits to complex symbol
 * @param bits pointer to input bitsequence
 * @param S_out pointer to complex symbol */
inline void modulate_16QAM(char *bits, output_t *S_out)
{
	int k;
	k = ((bits[0] == 1) ? 1 : 0) <<3 | ((bits[1] == 1) ? 1 : 0) <<2 |
		((bits[2] == 1) ? 1 : 0) <<1 | ((bits[3] == 1) ? 1 : 0);
	*S_out = qam16_table[k];
}

/**
 * 64QAM modulation function
 * Converts bitsequence of 6 bits to complex symbol
 * @param bits pointer to input bitsequence
 * @param S_out pointer to complex symbol */
inline void modulate_64QAM(char *bits, output_t *S_out)
{
	int k;

	k = ((bits[0] == 1) ? 1 : 0) <<5 | ((bits[1] == 1) ? 1 : 0) <<4 |
		((bits[2] == 1) ? 1 : 0) <<3 | ((bits[3] == 1) ? 1 : 0) <<2
		| ((bits[4] == 1) ? 1 : 0) <<1 | ((bits[5] == 1) ? 1 : 0);
	*S_out = qam64_table[k];
}

/**
 * Returns the number of bits per symbol
 * @param modulation modulation type */
inline int get_bits_per_symbol(int modulation)
{
	switch(modulation)
	{
		case BPSK:
			return 1;
		case QPSK:
			return 2;
		case QAM16:
			return 4;
		case QAM64:
			return 6;
		default:
			printf("Unknown modulation %d\n",modulation);
			return 0;
	}
}
