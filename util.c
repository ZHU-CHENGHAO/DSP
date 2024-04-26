/*
 *  ======== util.c ========
 *  Group :			4
 *
 *  Name: 			Chenghao Zhu
 *  Student number: 1928477
 *
 *  Name:			Yumo Liu
 *  Student number: 1929942
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gtz.h"

int tdiff,tdiff_final;

int sample, gtz_out[8];
int flag = 0;

short coef[8] =
		{ 0x6D02, 0x68AD, 0x63FC, 0x5EE7, 0x4A70, 0x4090, 0x3290, 0x23CE }; // goertzel coefficients
void task1_dtmfDetect();
void task2_dtmfGenerate(char* keys);
extern short* buffer;

void task1_dtmfDetect() {
	int i, n;
	int x, y;
	int max_x, max_y;
	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
	char result[8];

	for(n=0;n<8;n++) {
		while (!flag) Task_sleep(20);
		/* TODO 3. Complete code to detect the 8 digits based on the GTZ output */
		/* ========================= */
		max_x = gtz_out[0];
		max_y = gtz_out[4];
		x = 0;
		y = 0;

		for (i = 0; i < 4; i++)
		{
			if (gtz_out[i] > max_x) {
				max_x = gtz_out[i];
				x = i;
			}
			if (gtz_out[i+4] > max_y) {
				max_y = gtz_out[i+4];
				y = i;
			}
		}
		result[n] = pad[x][y];
		/* result[n] = ... */
		/* ========================= */
		printf("Element in gtz_out is : %d\n", gtz_out[n]);
		printf("%c\n", result[n]);
		flag = 0;
	}
	printf("\nDetection finished\n");
//	printf("Time taken for floating-point implementation = %d cycles \n",tdiff);				// Uncomment this line when testing floating-point implementation
//	printf("Time taken for fixed-point implementation using intrinsic = %d cycles \n",tdiff);	// Uncomment this line when testing fixed-point implementation with intrinsic approach
//	printf("Time taken for fixed-point implementation using switches = %d cycles \n",tdiff);	// Uncomment this line when testing fixed-point implementation with switches approach
	printf("Time taken for C-based GTZ Feedback loop = %d cycles \n", tdiff);					// Uncomment this line and next line below
	printf("Time taken for C-based GTZ Feedforward time = %d cycles \n",tdiff_final);			// testing C-implemented GTZ algorithm
	printf("Finished\n");
}
