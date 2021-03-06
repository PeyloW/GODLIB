#ifndef	INCLUDED_STRING_H
#define	INCLUDED_STRING_H

/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	<GODLIB/BASE/BASE.H>


/* ###################################################################################
#  STRUCTS
################################################################################### */

typedef	struct	sString
{
	char *	mpChars;
	U32		mCharCount;
} sString;


/* ###################################################################################
#  PROTOTYPES
################################################################################### */

sString *	String_Create( const char * apChars );
sString *	String_Create2( const char * apChars0, const char * apChars1 );
void		String_Destroy( sString * apString );

void		String_Append( sString * apString, const char * apChars );
void		String_Cat( sString * apDst, const sString * apSrc0, const sString * apSrc1 );
void		String_CharInsert( sString * apString, const U16 aIndex, const U8 aChar );
void		String_CharRemove( sString * apString, const U16 aIndex );
sString *	String_Clone( const sString * apString );
void		String_Copy( sString * apDst, const sString * apSrc );
U32			String_GetCharCount( const sString * apString );
void		String_Prepend(  sString * apString, const char * apChars );
void		String_Update( sString * apString, const char * apChars );
void		String_Update2( sString * apString, const char * apChars0, const char * apChars1 );

U32			String_StrLen( const char * apString );
void		String_StrCpy( char * apDst, const char * apSrc );
void		String_StrCpy2( char * apDst, const char * apSrc, const U32 aDstLen );
void		String_StrCat( char * apDst, const char * apSrc0, const char * apSrc1 );
U8			String_StrCmp( const char * apStr0, const char * apStr1 );
U8			String_StrCmpi( const char * apStr0, const char * apStr1 );
S32			String_ToValue( const char * apString );


/* ################################################################################ */

#endif	/* INCLUDED_STRING_H */
