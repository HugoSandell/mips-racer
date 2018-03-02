/*
	u32helpers.h
	Created on Feb 27, 2018 by Hugo Sandell
*/
#ifndef U32HELPERS_H
#define U32HELPERS_H
#include <stdint.h>

extern void savebyte( char s[], void * a );
extern void saveword( char s[], void * a );
extern void saveinfo( char s[], void * ap, size_t ns );
extern void show_debug_info( void );
extern void u32init( void );

extern void display_image(int x, const uint8_t *data);
extern void display_init(void);
extern void display_string(int line, char *s);
extern void display_update_debug(void);
extern void display_string(int line, char *s);
extern char * itoaconv( int num );

#endif //U32HELPERS_H
