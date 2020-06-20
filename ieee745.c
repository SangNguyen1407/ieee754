#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define SIGN_LEN     1
#define EXPONENT_LEN 8
#define MANTISSA_LEN 23
#define EXPBIAS      0x7F
#define BITLEN       (SIGN_LEN + EXPONENT_LEN + MANTISSA_LEN)
#define TRUE         1
#define FALSE        0


int int2bin( int changeValue, char *exponentValue)
{
	int i = EXPONENT_LEN - 1;
	int s = 0;
	int value = 0;
	for( s = 0;s <EXPONENT_LEN+1; s++){
		exponentValue[s] = 0;
	}
	s = 0;
	while (i >=0){
		value = 0x1 << i --;
		if (value & changeValue){
			exponentValue[ s++] = 1;
		}
		else{
			exponentValue[ s++] = 0;

		}
	}
	
}

int getExponent( int changeValue, char *buffer)
{
	int2bin(changeValue, buffer);
	int e = 0;
	for( int i = 1;i < EXPONENT_LEN+1; i++){
		if (buffer[i] == 1){
			e = EXPONENT_LEN - i-1;
			break;
		}
	}
	return e;
}

int frac2bin ( int changeValue, char *fracValueArr)
{
	int i = EXPONENT_LEN - 1;
	int s = 0;
	float value = 1.0;
	float fracValue = 0.0;
	int e = 0;
	float value_temp = 0.0;
	
	for( s = 0 ;s < MANTISSA_LEN; s++){
		fracValueArr[s] = 0;
	}
	value_temp = (float) changeValue;
	while (value_temp > 1){
		value_temp = value_temp / 10;
		e ++;
	}
	printf ("\n");
	s = 0;
	while (i < MANTISSA_LEN){
		value = value*2;
		if ((1/value) > value_temp){
			fracValueArr[ s++] = 0;
		}
		else if ((fracValue + 1/(value)) > value_temp){
			fracValueArr[ s++] = 0;
		}			
		else {
			fracValue = fracValue + 1/(value); 
			fracValueArr[ s++] = 1;

		}
		
		if (fracValue == value_temp){
			break;
		}
		i++;
	}
	
	return e;
}
	
int pack754_32(char *floatStr, char *floatingToIntValue, size_t bitsLen)
{
	unsigned long int intPart; // interge part
	unsigned long int fracPart;// fraction part
	unsigned long int mantiPart;
	
	unsigned long int temp;
	unsigned long int valuePos;
	unsigned long int fracPartLength;
	unsigned long int intPartHighestBit = 1;
	unsigned long int fracPartPowTen = 1;
	
	char roundBit;
	char stickyBit;
	char expPart = 0;
	
	// Get sign
	if (floatStr[0] == '-'){
		floatStr++;
		floatingToIntValue[0] = 1;
	}
	else{
		if (floatStr[0] == '+'){
			floatStr++;
		}
		floatingToIntValue[0] = 0;
	}
	
	// Change string to binary
		// Get intPart and fracPart
	if (sscanf(floatStr, "%lu.%lu", &intPart, &fracPart) == EOF || 
	    bitsLen < BITLEN + 1){
		return FALSE;
	}

		// Change string intPart to binary intPart
	char *value = malloc ( EXPONENT_LEN );
	char *exponentValue = malloc ( EXPONENT_LEN );
	char *fracValue = malloc ( MANTISSA_LEN );
	int e = getExponent(intPart, value);
	int expValue = e + 127;
	int pos = 1;
	int2bin(expValue, exponentValue);
	for( int s = 1 ;s < EXPONENT_LEN+1; s++){
		floatingToIntValue[pos++] = exponentValue[s-1];
	}

	for( int s = EXPONENT_LEN - e ;s < EXPONENT_LEN; s++){
		floatingToIntValue[pos++] = value[s];
	}
	frac2bin(fracPart, fracValue);
	int len = BITLEN - pos;
	for( int s = 0 ;s < len; s++){
		floatingToIntValue[pos++] = fracValue[s];
	}
	
	for (int s = 0;s < BITLEN ; s++){
		printf ("%d",floatingToIntValue[s] );
		if ((s + 1) % 4 == 0 && (s + 1) != 0){
			printf (" ");
		}
	}
	return TRUE;
}


int main(int argv, char **argc)
{
	char *floatStr = "12.345";
	char *floatingToIntValue = malloc (BITLEN + 1); // BITLEN + "."
	
	
	// Convert 19.5 into IEEE 754 binary32 format
	if (pack754_32(floatStr, floatingToIntValue, BITLEN + 1))
	{
		printf ("\ntest.%s\n", floatingToIntValue);
	}

	return 0;
}

