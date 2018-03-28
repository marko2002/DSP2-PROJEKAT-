#include "processing.h"
#include "iir.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



void calculateShelvingCoeff(float c_alpha, Int16* output)
{
	/* Your code here */

	output[0]=c_alpha*32767;//A0
	output[1]=-1 * 32767;//A1
	output[2]= 1 * 32767;//B0
	output[3]=-c_alpha*32767;//B1

}

void calculatePeekCoeff(float c_alpha, float c_beta, Int16* output)
{
	/* Your code here */
	output[0]=c_alpha * 32767;//A0
	output[1]=(-c_beta)*(1+c_alpha)*16384;//A1/2
	output[2]=1 * 32767;//A2
	output[3]=1 * 32767;//B0
	output[4]=(-c_beta)*(1+c_alpha)*16384;//B1/2
	output[5]=c_alpha * 32767;//B2

}

Int16 shelvingHP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 bufferA1;
	Int16 output;


	bufferA1=first_order_IIR(input,coeff,x_history,y_history);
	output = _smpy(k,input/2 + bufferA1/2)+input/2 - bufferA1/2;

	if (output>32766){
		return 32766;
	}else if(output<-32766){

		return -32766;
	}else{

		return output;
	}




	return 0;
}

Int16 shelvingLP(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 bufferA2;
	Int16 output;

	bufferA2 = first_order_IIR(input,coeff,x_history,y_history);
	output = input/2 + bufferA2/2+ _smpy(k,input/2 - bufferA2/2);

	if (output>32766){
		return 32766;
	}else if(output<-32766){

		return -32766;
	}else{

		return output;
	}



	return 0;
}

Int16 shelvingPeek(Int16 input, Int16* coeff, Int16* x_history, Int16* y_history, Int16 k)
{
	/* Your code here */

	Int16 bufferPeek;
	Int16 output;

	bufferPeek = second_order_IIR(input,coeff,x_history,y_history);
	output = input/2 + bufferPeek/2+ _smpy(k,input/2 - bufferPeek/2);

	if (output>32766){
		return 32766;
	}else if(output<-32766){

		return -32766;
	}else{

		return output;
	}



	return 0;
}
