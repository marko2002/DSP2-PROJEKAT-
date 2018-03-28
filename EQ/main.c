//////////////////////////////////////////////////////////////////////////////
// *
// * Predmetni projekat iz predmeta OAiS DSP 2
// * Godina: 2017
// *
// * Zadatak: Ekvalizacija audio signala
// * Autor:
// *                                                                          
// *                                                                          
/////////////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "ezdsp5535_sar.h"
#include "print_number.h"
#include "math.h"

#include "iir.h"
#include "processing.h"

/* Frekvencija odabiranja */
#define SAMPLE_RATE 8000L

#define PI 3.14159265

/* Niz za smestanje ulaznih i izlaznih odbiraka */
#pragma DATA_ALIGN(sampleBufferL,4)
Int16 sampleBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(sampleBufferR,4)
Int16 sampleBufferR[AUDIO_IO_SIZE];
Int16 diracBuffer[AUDIO_IO_SIZE];
Int16 outputBuffer[AUDIO_IO_SIZE];
Int16 historyX[AUDIO_IO_SIZE];
Int16 historyY[AUDIO_IO_SIZE];
Int16 outputCoeff[4];

#define value
float f[6] = {2*PI*320/SAMPLE_RATE,2*PI*790/SAMPLE_RATE,2*PI*430/SAMPLE_RATE,2*PI*3660/SAMPLE_RATE,2*PI*1880/SAMPLE_RATE,2*PI*6/SAMPLE_RATE};
void main( void )



{   

	int i;
    /* Inicijalizaija razvojne ploce */
    EZDSP5535_init( );

    /* Inicijalizacija kontrolera za ocitavanje vrednosti pritisnutog dugmeta*/
    EZDSP5535_SAR_init();

    /* Inicijalizacija LCD kontrolera */
    initPrintNumber();

	printf("\n Ekvalizacija audio signala \n");
		
    /* Inicijalizacija veze sa AIC3204 kodekom (AD/DA) */
    aic3204_hardware_init();
	
    /* Inicijalizacija AIC3204 kodeka */
	aic3204_init();

    aic3204_dma_init();
    
    /* Postavljanje vrednosti frekvencije odabiranja i pojacanja na kodeku */
    set_sampling_frequency_and_gain(SAMPLE_RATE, 0);

    diracBuffer[0]=16000;
    historyX[0]=0;
    historyY[0]=0;
    for(i = 1 ; i <AUDIO_IO_SIZE ; i++){
    	diracBuffer[i]=0;
        historyX[i]=0;
        historyY[i]=0;
    }

    float alpha,beta;

    //alpha =(1-sin(f[0]))/cos(f[0]);

    calculateShelvingCoeff(-0.3, outputCoeff);


    while(1)
    {
    	aic3204_read_block(sampleBufferL, sampleBufferR);

    	/* Your code here */

    	for(i=0;i<AUDIO_IO_SIZE; i++){
    		outputBuffer[i]=shelvingHP(diracBuffer[i],outputCoeff,historyX,historyY,8192 );
    	}

		aic3204_write_block(sampleBufferR, sampleBufferR);
	}

    	
	/* Prekid veze sa AIC3204 kodekom */
    aic3204_disable();

    printf( "\n***Kraj programa***\n" );
	SW_BREAKPOINT;
}


