#include <stdio.h>
#include <stdbool.h>

//Our division polynomial.
const unsigned int polynomial = 0x04C11DB7;
unsigned int reverseBits(unsigned int n)
{
	unsigned int reversed = 0;

	while (n > 0)
	{
		reversed  = reversed << 1;

		if (n & 1)
		{
			reversed = reversed ^ 1;
		}
		n = n >> 1;
	}
	return reversed;
}
// Takes in a REVERSED input
// Shifts the input bit by bit and applies modulo 2 addition per iteration.
// This is meant to simulate polynomial division where calculations are done
// in GF(2) or modulo 2 arithmetic. 
unsigned long int compute_crc( unsigned long input, int len)
{
	while (len > 0){
		len -= 1;
		if (input & 0x80000000)
		{
			input = polynomial ^ (input << 1);
		}
		else
		{
			input = input << 1;
		}
	}
	return input;
}

bool isTableCreated = false;
unsigned long crc_table[ 256 ]; // Table that stores partially calculated inputs.

unsigned long int compute_crc_WITHTABLE(unsigned char *input, int len)
{
  unsigned long crc = 0xFFFFFFFF;
  unsigned long tableIndex;
  
  for (int i = 0; i < len; i++)
  {
    tableIndex = 0xFF & (((unsigned long)input[i]) ^ crc);
    crc = (crc >> 8) ^ crc_table[tableIndex];
  }

  return 0xFFFFFFFF ^ crc;
}

unsigned long int crc_byte(unsigned long input)
{
  const unsigned int polynomial = 0xEDB88320;
  for (int k = 8; k > 0; k--)
  {
	  if (input & 1)
	  {
		  input = (input >> 1) ^ polynomial;
	  }
	  else
	  {
		  input = input >> 1;
	  }
  }

  return input;
}

bool createTable()
{
	if (!isTableCreated)
	{
		for (int i = 0; i < 256; i++)
		{
			crc_table[i] = crc_byte(i);
		}
	}
	return true;
}

int main(int argc, char *argv[]){
	unsigned long int input = 0x8242C200; // "ABC" backwards
	printf( "%lx\n", compute_crc(input, 24)); // 5A5B433A
	

	printf("\n%d\n", reverseBits(input));
	// Builds the table.
	// Sets variable so that the table won't need to be created on
	// subsequent runs.
	isTableCreated = createTable();

	unsigned char *input2 = "ABC";
	printf( "%lx\n", compute_crc_WITHTABLE(input2, 3)); //a3830348 // 5A5B433A
}
