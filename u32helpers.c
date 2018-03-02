/* u32helpers.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Updated 2015-08-30 by F Lundevall
   Latest update 2018-02-25 by Hugo Sandell

   For copyright and licensing, see file COPYING */

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <string.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipkitio.h"  /* Declarations of system-specific addresses etc */

/* Declare all functions in advance. */

void savebyte( char s[], void * a );
void saveword( char s[], void * a );
void saveinfo( char s[], void * ap, size_t ns );
void show_debug_info( void );
void u32init( void );

void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update_debug(void);
uint8_t spi_send_recv(uint8_t data);

char * itoaconv( int num );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

char textbuffer[4][16];

/* Declare a helper function which is local to this file */
static void num32asc( char * s, unsigned, int );

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr, 4 );
  num32asc( &textbuffer[2][6], *addr, 4 );
  display_update_debug();
}


void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;

	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}


void display_update_debug(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, unsigned n, int sz )
{
  int i;
  int bits;
  if( sz == 8 ) bits = 28;
  else if( sz == 4 ) bits = 28;
  else if( sz == 2 ) bits = 12;
  else if( sz == 1 ) bits = 4;
  else bits = 0;
  if( bits )
    for( i = bits; i >= 0; i -= 4 )
      *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
  else
    for( i = 0; i < 8; i += 1 )
      *s++ = "bad size"[ i ];
}

/*
 * itoa
 *
 * Simple conversion routine
 * Converts binary to decimal numbers
 * Returns pointer to (static) char array
 *
 * The integer argument is converted to a string
 * of digits representing the integer in decimal format.
 * The integer is considered signed, and a minus-sign
 * precedes the string of digits if the number is
 * negative.
 *
 * This routine will return a varying number of digits, from
 * one digit (for integers in the range 0 through 9) and up to
 * 10 digits and a leading minus-sign (for the largest negative
 * 32-bit integers).
 *
 * If the integer has the special value
 * 100000...0 (that's 31 zeros), the number cannot be
 * negated. We check for this, and treat this as a special case.
 * If the integer has any other value, the sign is saved separately.
 *
 * If the integer is negative, it is then converted to
 * its positive counterpart. We then use the positive
 * absolute value for conversion.
 *
 * Conversion produces the least-significant digits first,
 * which is the reverse of the order in which we wish to
 * print the digits. We therefore store all digits in a buffer,
 * in ASCII form.
 *
 * To avoid a separate step for reversing the contents of the buffer,
 * the buffer is initialized with an end-of-string marker at the
 * very end of the buffer. The digits produced by conversion are then
 * stored right-to-left in the buffer: starting with the position
 * immediately before the end-of-string marker and proceeding towards
 * the beginning of the buffer.
 *
 * For this to work, the buffer size must of course be big enough
 * to hold the decimal representation of the largest possible integer,
 * and the minus sign, and the trailing end-of-string marker.
 * The value 24 for ITOA_BUFSIZ was selected to allow conversion of
 * 64-bit quantities; however, the size of an int on your current compiler
 * may not allow this straight away.
 */
#define ITOA_BUFSIZ ( 24 )
char * itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}


/* Here follows a lot of complicated code to display stuff */

#define NUMENTRIES (64)
#define LINELENGTH (16)
char str[NUMENTRIES][LINELENGTH];
unsigned int  add[NUMENTRIES];
unsigned int  dat[NUMENTRIES];
unsigned int  siz[NUMENTRIES];
int  flg[NUMENTRIES];
static int numused = 0;

void savebyte( char s[], void * a )
{
  saveinfo( s, a, 1 );
}

void saveword( char s[], void * a )
{
  saveinfo( s, a, 4 );
}

void saveinfo( char s[], void * ap, size_t ns )
{
  char c;
  int i;
  int a = (unsigned int) ap;
  int n = (unsigned int) ns;
  if( numused > NUMENTRIES ) return;
  for( i = 0; i < LINELENGTH; i += 1 )
  {
    c = *s;
    s += 1;
    if( c )
      str[ numused ][ i ] = c;
    else
      str[ numused ][ i ] = 0;
  }
  str[ numused ][ LINELENGTH - 1 ] = 0; /* Always terminate a string */
  add[ numused ] = a;
  siz[ numused ] = n;
  if( n == 8 )
  {
    if( a & 7 )
      flg[ numused ] = 2; /* non-aligned */
    else
    {
      dat[ numused ] = *( (unsigned long long *) ap);
      flg[ numused ] = 1; /* aligned */
    }
  }if( n == 4 )
  {
    if( a & 3 )
      flg[ numused ] = 2; /* non-aligned */
    else
    {
      dat[ numused ] = *( (unsigned int *) ap);
      flg[ numused ] = 1; /* aligned */
    }
  }else if( n == 2 )
  {
    if( a & 1 )
      flg[ numused ] = 2; /* non-aligned */
    else
    {
      dat[ numused ] = (unsigned int) *( (uint16_t *) ap);
      flg[ numused ] = 1; /* aligned */
    }
  }else if( n == 1 )
  {
    dat[ numused ] = (unsigned int) *( (uint8_t *) ap);
    flg[ numused ] = 1; /* aligned */
  }
  else flg[ numused ] = 4; /* weird size */
  numused += 1;
  return;
}


int current = 0;
void show_debug_info( void )
{
  char tmp[ LINELENGTH ];
  char * cp;
  char * tp;

  *( (volatile int *) 0xbf8860c8 ) = ( 0x3f << 5 ); /* ugly */

  if( numused <= 0 )
  {
    display_string( 3, "empty" );
    display_update();
    return;
  }


    display_string( 0, str[ current ] );
    display_string( 1, "Addr" );
    num32asc( &textbuffer[1][6], add[ current ], 4 );
    if( flg[ current ] & 6 && 0)
    {
      if( flg[ current ] & 2 ) display_string( 2, "Unaligned data" );
      else display_string( 2, "Bad data-size");
    }
    else
    {
      display_string( 2, "Data" );
      num32asc( &textbuffer[2][6], *((unsigned *)add[current]), siz[ current ] );
    }
    tp = tmp;
    cp = itoaconv( current + 1 );
    if( *cp ) *tp++ = *cp++;
    if( *cp ) *tp++ = *cp++;
    if( *cp ) *tp++ = *cp++;
    *tp++ = 0x20;
    *tp++ = 'o';
    *tp++ = 'f';
    *tp++ = 0x20;
    cp = itoaconv( numused );
    if( *cp ) *tp++ = *cp++;
    if( *cp ) *tp++ = *cp++;
    if( *cp ) *tp++ = *cp++;
    *tp = 0;
    display_string( 3, tmp );
    display_update_debug();

      if(!get_switch(SW3) || !get_switch(SW4)) return;
      if( ( *( (volatile int *) 0xbf8860d0) >> 5 ) & 2 )
      {
	       current += 1;
      }

    if( current < 0 ) current = numused - 1;
    if( current >= numused ) current = 0;
    while( ( *( (volatile int *) 0xbf8860d0) >> 5 ) & 2 );
}
