/*
 *  ======== gtz.c ========
 *  Group:			4
 *
 *  Name:			Chenghao Zhu
 *  Student number: 1928477
 *
 *  Name:			Yumo Liu
 *  Student number: 1929942
 */


/* type your names, emails and group number here */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

#include "c6x.h"
#include <c6x.h>

#define COUNT 205

void clk_SWI_Read_Data(UArg arg0);
void clk_SWI_GTZ_All_Freq(UArg arg0);

extern void task0_dtmfGen(void);
extern void task1_dtmfDetect(void);

extern int dtmf_opt_float(short *m, short *n, int count);
extern int dtmf_opt_intrinsics(short *m, short *n, int count);
extern int dtmf_opt_switches(short *m, short *n, int count);

extern int sample, tdiff, tdiff_final, gtz_out[8];
extern short *coef[8];
extern int flag;

short data[NO_OF_SAMPLES];
short *buffer, *x, input;
int i;

/*
 *  ======== main ========
 */
int main() {
	System_printf("\n System Start\n");
	System_flush();

	/* Read binary data file */
	FILE* fp = fopen("../data.bin", "rb");
	if(fp==0) {
		System_printf("Error: Data file not found\n");
		System_flush();
		return 1;
	}
	fread(data, 2, NO_OF_SAMPLES, fp);
	buffer = (short*)malloc(2*8*10000);

	/* Create a Clock Instance */
	Clock_Params clkParams;

	/* Initialise Clock Instance with time period and timeout (system ticks) */
	Clock_Params_init(&clkParams);
	clkParams.period = 1;
	clkParams.startFlag = TRUE;

	/* Instantiate ISR for tone generation  */
	Clock_create(clk_SWI_Read_Data, TIMEOUT, &clkParams, NULL);

	/* Instantiate 8 parallel ISRs for each of the eight Goertzel coefficients */
	Clock_create(clk_SWI_GTZ_All_Freq, TIMEOUT, &clkParams, NULL);

	/* Start SYS_BIOS */
	BIOS_start();
}

/*
 *  ====== clk_SWI_Generate_DTMF =====
 *  Dual-Tone Generation
 *  ==================================
 */
void clk_SWI_Read_Data(UArg arg0) {
	static int tick;
	tick = Clock_getTicks();
	sample = data[tick%NO_OF_SAMPLES];
}

/*
 *  ====== clk_SWI_GTZ =====
 *  gtz sweep
 *  ========================
 */
void clk_SWI_GTZ_All_Freq(UArg arg0) {
	// define variables for timing
	static int start, stop;

   	//Record start time
	start = Timestamp_get32();

	static int Goertzel_Value = 0;
	input = (short) (sample);

	x = &input;

	for (i=0; i<7; i++){

		// Floating point implementation
		//Goertzel_Value = dtmf_opt_float(x, coef[i], COUNT);

		// Fixed point implementation using intrinsics and compiler switches
	    Goertzel_Value = dtmf_opt_intrinsics(x, coef[i], COUNT);
	    //Goertzel_Value = dtmf_opt_switches(x, coef[i], COUNT);
	    gtz_out[i] = sqrt(Goertzel_Value);
	}

	stop = Timestamp_get32();

	tdiff = stop - start;

    flag = 1;
}
