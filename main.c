/*
 * Group:			4
 *
 * Name:			Chenghao Zhu
 * Student number:	1928477
 *
 * Name: 			Yumo Liu
 * Student number:	1929942
 */

#include "gtz.h"
#include <stdio.h>
#include <math.h>

//#define SAMPLING_RATE 8000  // Replace with your actual sampling rate
#define NUM_TARGETS 8
#define NUM_SAMPLES 205

// Global variables
float targetFrequencies[NUM_TARGETS] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
float FrequCoef[NUM_TARGETS]={1.703275,1.635585,1.562297,1.482867,1.163138,1.008835,0.790074,0.559454};//Define 8 frequencies and their corresponding coefficients.
float magnitudes[NUM_TARGETS];
float samples[NUM_SAMPLES];

int xaxis=0;
int yaxis=0;

char digit;

// Function prototypes
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[]);
void Frequency_detection(float magnitudes[]);
void Generate_tones(char digit);

// Function to perform the Goertzel algorithm for multiple frequencies
void goertzel_multi(const float samples[], int numSamples, float targetFrequencies[])//
{
	// to be completed
	float freqDelay[NUM_TARGETS]={0.0};
	float freqDelay_1[NUM_TARGETS]={0.0};
	float freqDelay_2[NUM_TARGETS]={0.0};
	int i=0;
	int j=0;
	float freqSquare_1=0.0;
	float freqSquare_2=0.0;
	float freqCoef12=0.0;
	for(i=0;i<NUM_TARGETS;++i)
	{
		for(j=0;j<numSamples;++j)
		{
			freqDelay[i]=samples[j]+FrequCoef[i]*freqDelay_1[i]-freqDelay_2[i];//Because of the floating point operation, there is no need to perform a shift operation, just calculate directly
			freqDelay_2[i]=freqDelay_1[i];
			freqDelay_1[i]=freqDelay[i];
		}
		freqSquare_1=freqDelay_1[i]*freqDelay_1[i];
		freqSquare_2=freqDelay_2[i]*freqDelay_2[i];
		freqCoef12=FrequCoef[i]*freqDelay_1[i]*freqDelay_2[i];
		magnitudes[i]=sqrt(freqSquare_1+freqSquare_2-freqCoef12);
		printf("Magnitude for frequency is %f, The magnitude is %f\n",targetFrequencies[i],magnitudes[i]);
	}
}
// Function to detect the maximum frequency and interpret the key pressed
void Frequency_detection(float magnitudes[])
{
	// to be completed
	char pad[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
	int xaxis,yaxis,max_xaxis,max_yaxis,i;
	max_xaxis = magnitudes[0];
	max_yaxis = magnitudes[4];
	xaxis = 0;
	yaxis = 0;
	for (i = 0; i < 4; i++)
	{
		if (magnitudes[i] > max_xaxis)
		{
			max_xaxis = magnitudes[i];
			xaxis = i;
		}
		if (magnitudes[i+4] > max_yaxis)
		{
			max_yaxis = magnitudes[i+4];
			yaxis = i;
		}
	}
	printf("The key you pressed is %c\n", pad[xaxis][yaxis]);
}

// Function to generate tones based on the pressed key
void Generate_tones(char digit)
{
	// to be completed
	int i=0;
	for(i=0;i<NUM_SAMPLES;++i)
	{
		samples[i]=cos(2.0*PI*targetFrequencies[xaxis]*TICK_PERIOD*i)+cos(2.0*PI*targetFrequencies[yaxis]*TICK_PERIOD*i);//Add up the horizontal and vertical frequencies.
	}
}
int main() {
    while (1) {
        printf("Type a key and press return:\n");
        scanf("\n%c", &digit);
        xaxis=-1;yaxis=-1;
		if ('#'==digit)
		{
			xaxis=3;yaxis=6;
		}
		else if ('*'==digit)
		{
			xaxis=3;yaxis=4;
		}
		else if ('0'==digit)
		{
			xaxis=3;yaxis=5;
		}
		else if ('1'==digit)
		{
			xaxis=0;yaxis=4;
		}
		else if ('2'==digit)
		{
			xaxis=0;yaxis=5;
		}
		else if ('3'==digit)
		{
			xaxis=0;yaxis=6;
		}
		else if ('4'==digit)
		{
			xaxis=1;yaxis=4;
		}
		else if ('5'==digit)
		{
			xaxis=1;yaxis=5;
		}
		else if ('6'==digit)
		{
			xaxis=1;yaxis=6;
		}
		else if ('7'==digit)
		{
			xaxis=2;yaxis=4;
		}
		else if ('8'==digit)
		{
			xaxis=2;yaxis=5;
		}
		else if ('9'==digit)
		{
			xaxis=2;yaxis=6;
		}
		else if ('A'==digit)
		{
			xaxis=0;yaxis=7;
		}
		else if ('B'==digit)
		{
			xaxis=1;yaxis=7;
		}
		else if ('C'==digit)
		{
			xaxis=2;yaxis=7;
        }
		else if ('D'==digit)
		{
			xaxis=3;yaxis=7;
		}

		else
		{
			printf("You enter the wrong character! Please enter the right character\n");
		}
		if(xaxis==-1 || yaxis==-1)
			continue;
		Generate_tones(digit);

        // Example usage

        // Call the function to calculate magnitudes for each frequency
        goertzel_multi(samples, NUM_SAMPLES, targetFrequencies);

        // Detect the maximum frequency and interpret the key pressed
        Frequency_detection(magnitudes);
    }


}
