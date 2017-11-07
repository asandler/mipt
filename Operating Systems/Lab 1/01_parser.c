/*
	This is a shell parser

	(c) Andrew Sandler, 2011
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define ERR_OK		0
#define ERR_NOT_OK	1
#define ERR_OUT_MEM	2
#define ERR_ILLEG_QS	3
#define ERR_CASE_CHARS	4
#define PARSE_SEPARATOR	5
#define SEPARATOR	6
#define PUNCT		7
#define QUOTE		8
#define LETTER		9
#define NOT_A_CHAR	10


struct string {
	char *data;
	int length, alloc_size;
};

struct string puncts, separators, quotes;

void init_string( struct string *s ) {
	s->alloc_size = 16;
	s->data = malloc( sizeof( char ) * s->alloc_size );
	s->data[0] = 0;
	s->length = 0;
}

int add_to_string( struct string *s, char c ) {
	if ( s->length == s->alloc_size -1 )
		if ( ( s->data = realloc( s->data, s->alloc_size *2 ) ) == NULL )
			return ERR_OUT_MEM;
		else
			s->alloc_size *= 2;
	s->data[s->length++] = c;
	s->data[s->length] = 0;
	return ERR_OK;
}

void init_chars() {
	char c = 'A';

	init_string( &separators );
	init_string( &puncts );
	init_string( &quotes );

	add_to_string( &separators, ' ' );
	add_to_string( &quotes, '"' );
	add_to_string( &quotes, '\'' );

	for ( c = ' '; c <= '~'; c++ )
		if ( ! ( strchr( separators.data, c ) || strchr( quotes.data, c ) ) )
			if ( c < 'A' || ( c > 'Z' && c < 'a' ) || ( c > 'z' ) )
				add_to_string( &puncts, c );
}

void erase_chars() {
	free( separators.data );
	free( puncts.data );
	free( quotes.data );
}

int get_string( struct string *s ) {
	char c;
	while ( ( c = getchar() ) != EOF && c != '\n' )
		if ( add_to_string(s, c) != ERR_OK )
			return ERR_OUT_MEM;
	return ERR_OK;
}

void erase_string( struct string *s ) {
	free(s->data);
}

void print_string( struct string *s ) {
	int t = 0;
	for ( ; t < s->length; t++ )
		if ( s->data[t] != PARSE_SEPARATOR )
			printf("%c", s->data[t]);
		else
			printf("\n");
	printf("\n");
}

int print_error( int t ) {
	switch (t) {
		case ERR_OUT_MEM: fprintf(stderr, "parser: Read error: Can't read the input string (probably it's too long)\n"); return ERR_NOT_OK;
		case ERR_ILLEG_QS: fprintf(stderr, "parser: Parse error: Illegal quote sequience\n"); return ERR_NOT_OK;
		case ERR_CASE_CHARS: fprintf(stderr, "parser: Parse error: Input string contains case letters\n"); return ERR_NOT_OK;
		default: return ERR_OK;
	}
}

int char_type( char t ) {
	if ( strchr( separators.data, t ) ) return SEPARATOR;
	if ( strchr( puncts.data, t ) ) return PUNCT;
	if ( strchr( quotes.data, t ) ) return QUOTE;
	if ( t >= 0 && t < 32 ) return NOT_A_CHAR;
	return LETTER;
}

void refresh( struct string *s ) {
	if ( s->length > 0 && s->data[s->length -1] != PARSE_SEPARATOR )
		add_to_string( s, PARSE_SEPARATOR );
}

int parse_string( struct string *s, struct string *parsed ) {
	int t = 0, c_type = NOT_A_CHAR, c_type_old;
	char quote = 0;
	for ( ; t < s->length; t++ ) {
		c_type_old = c_type;
		c_type = char_type( s->data[t] );
		switch ( c_type ) {
			case SEPARATOR:
				if ( !quote )
					refresh( parsed );
				else
					add_to_string( parsed, s->data[t] );
				break;
			case PUNCT:
				if ( !quote )
					refresh( parsed );
				add_to_string( parsed, s->data[t] );
				break;
			case QUOTE:
				if ( !quote ) {
					refresh( parsed );
					quote = s->data[t];
					add_to_string( parsed, quote );
				} else {
					add_to_string( parsed, s->data[t] );
					if ( quote == s->data[t] )
						quote = 0;
				}
				break;
			case LETTER:
				if ( ! ( quote || c_type_old == LETTER ) )
						refresh ( parsed );
					add_to_string( parsed, s->data[t] );
				break;
			default:
				return ERR_CASE_CHARS;
		}
	}
	if ( quote ) return ERR_ILLEG_QS;
	return ERR_OK;
}

int main() {
	struct string input, parsed;

	init_chars();
	init_string(&input);
	init_string(&parsed);

//	print_string(&separators);
//	print_string(&quotes);
//	print_string(&puncts);

	if ( print_error( get_string(&input) ) == ERR_OK )
		if ( print_error( parse_string(&input, &parsed) ) == ERR_OK )
			print_string(&parsed);

	erase_chars();
	erase_string(&input);
	erase_string(&parsed);

	return 0;
}
