#include <stdio.h>
#include <stdbool.h>

// Takes in a REVERSED input
// Shifts the input bit by bit and applies modulo 2 addition per iteration.
// This is meant to simulate polynomial division where calculations are done
// in GF(2) or modulo 2 arithmetic. 
unsigned long int compute_crc( unsigned long input, int len)
{
	const unsigned int polynomial = 0x04C11DB7;
	while (len--){
		input = (input & 0x80000000) ? polynomial ^ (input << 1) : (input << 1);
	}
	return input;
}

unsigned long crc_table[ 255 ]; // Table that stores partially calculated inputs. 

unsigned long int compute_crc_WITHTABLE( unsigned char *input,
                               int len)
{
  int i;
  unsigned long crc = 0xFFFFFFFF;

  for ( i = 0; i < len; i++ )
  {
    crc = crc_table[ ( crc ^ input[ i ] ) & 0xff ] ^ ( crc >> 8 );
  }

  return crc ^ 0xFFFFFFFF;
}
unsigned long int crc_byte( unsigned long input,
                            unsigned long divisor )
{
  int k;

  for ( k = 8; k; k-- )
  {
    input = input & 1 ? (input >> 1) ^ divisor : input >> 1;
  }

  return input;
}


int main(int argc, char *argv[]){
	unsigned long int input = 0x8242C200; // "ABC" backwards & left aligned

  printf( "%lx\n", compute_crc( input, 24)); // 5A5B433A
  
  // Builds the table.
  for (int i = 0; i < 255; i++ )
  {
    crc_table[ i ] = crc_byte( i, 0x04C11DB7 );
  }

  printf( "%lx\n", compute_crc_WITHTABLE( input, 24)); // 5A5B433A
  
}
