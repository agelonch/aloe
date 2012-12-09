
#define INCLUDE_DEFS_ONLY
#include "gen_modulator.h"


/* LTE specifies QPSK, 16QAM and 64QAM for data and BPSK for some control channels */


/**@ingroup gen_modulator
 * @{
 */
#define BPSK    0
#define QPSK    1
#define QAM16   2
#define QAM64   3

/**@} */

/* The symbols are specified in [3GPP TS 36.211 version 10.5.0 Release 10, Section 7] */
/* from which the symbols levels follow: */
#define BPSK_LEVEL      1/sqrt(2)		//OSSIE: 1   	///< BPSK amplitude (RMS=10000)

#define QPSK_LEVEL      1/sqrt(2)		//OSSIE: 1   	///< QPSK amplitude (RMS=10000)

#define QAM16_LEVEL_1	1/sqrt(10)		//OSSIE: 0.3162  ///< Low 16-QAM amplitude (RMS=10000)
#define QAM16_LEVEL_2	3/sqrt(10)		//OSSIE: 0.9487  ///< High 16-QAM amplitude (RMS=10000)

#define QAM64_LEVEL_1	1/sqrt(42)
#define QAM64_LEVEL_2	3/sqrt(42)
#define QAM64_LEVEL_3	5/sqrt(42)
#define QAM64_LEVEL_4	7/sqrt(42)

#define QAM64_LEVEL_x	2/sqrt(42)		/* this is not an QAM64 level, but, rather, an auxiliary value that can be used for computing the symbol from the bit sequence */

/* Thresholds for Demodulation */
/* Assume perfect amplitude and phase alignment.
 *  Check threshold values for real case
 *  or implement dynamic threshold adjustent as a function of received symbol amplitudes */
#define QAM16_THRESHOLD		2/sqrt(10) 				//OSSIE: 0.6324    ///< 16-QAM threshold for RMS=10000 signal

#define QAM64_THRESHOLD_1	2/sqrt(42)
#define QAM64_THRESHOLD_2	4/sqrt(42)
#define QAM64_THRESHOLD_3	6/sqrt(42)


/* Moduation Mapper/Demapper functions
 * INIT phase */
void set_BPSKtable(void);
void set_QPSKtable(void);
void set_16QAMtable(void);
void set_64QAMtable(void);

int get_bits_per_symbol(int modulation);

void modulate_BPSK(input_t *bits, output_t *S_out);
void modulate_QPSK(input_t *bits, output_t *S_out);
void modulate_16QAM(input_t *bits, output_t *S_out);
void modulate_64QAM(input_t *bits, output_t *S_out);
