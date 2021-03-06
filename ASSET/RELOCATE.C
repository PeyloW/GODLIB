/* ###################################################################################
#  INCLUDES
################################################################################### */

#include	"RELOCATE.H"

#include	"ASSET.H"

#include	<GODLIB/DEBUGLOG/DEBUGLOG.H>
#include	<GODLIB/MEMORY/MEMORY.H>


/* ###################################################################################
#  DATA
################################################################################### */

sRelocater *	gpRelocaters = 0;


/* ###################################################################################
#  CODE
################################################################################### */

/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_Init( void )
* ACTION   : RelocaterManager_Init
* CREATION : 30.11.2003 PNK
*-----------------------------------------------------------------------------------*/

void	RelocaterManager_Init( void )
{
	gpRelocaters = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_DeInit( void )
* ACTION   : RelocaterManager_DeInit
* CREATION : 30.11.2003 PNK
*-----------------------------------------------------------------------------------*/

void	RelocaterManager_DeInit( void )
{
	sRelocater *	lpReloc;
	sRelocater *	lpRelocNext;

	lpReloc = gpRelocaters;

	while( lpReloc )
	{
		lpRelocNext = lpReloc->mpNext;
		mMEMFREE( lpReloc );
		lpReloc     = lpRelocNext;
	}

	gpRelocaters = 0;
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Relocater_Create( const char * apExt,fReloc aIsType,fReloc aDoInit,fReloc aDoDeInit,fReloc aDoRelocate,fReloc aDoDelocate )
* ACTION   : Relocater_Create
* CREATION : 30.11.2003 PNK
*-----------------------------------------------------------------------------------*/

sRelocater *	Relocater_Create( const char * apExt,fReloc aIsType,fReloc aDoInit,fReloc aDoDeInit,fReloc aDoRelocate,fReloc aDoDelocate )
{
	sRelocater *	lpReloc;

	DebugLog_Printf1( "Relocater_Create() %s", apExt );

	lpReloc = (sRelocater*)mMEMCALLOC( sizeof( sRelocater ) );

	if( lpReloc )
	{
		lpReloc->mExtID     = Asset_BuildHash( apExt );
		lpReloc->DoInit     = aDoInit;
		lpReloc->DoDeInit   = aDoDeInit;
		lpReloc->DoDelocate = aDoDelocate;
		lpReloc->DoRelocate = aDoRelocate;
		lpReloc->IsType     = aIsType;
		lpReloc->mpNext     = gpRelocaters;

		gpRelocaters        = lpReloc;
	}

	return( lpReloc );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : Relocater_Destroy( sRelocater * apReloc )
* ACTION   : Relocater_Destroy
* CREATION : 30.11.2003 PNK
*-----------------------------------------------------------------------------------*/

void	Relocater_Destroy( sRelocater * apReloc )
{
	sRelocater *	lpReloc;
	sRelocater *	lpRelocLast;

	lpReloc     = gpRelocaters;
	lpRelocLast = 0;

	while( (lpReloc) && (lpReloc != apReloc) )
	{
		lpRelocLast = lpReloc;
		lpReloc     = lpReloc->mpNext;
	}

	if( lpReloc == apReloc )
	{
		if( lpRelocLast )
		{
			lpRelocLast->mpNext = lpReloc->mpNext;
		}
		else
		{
			gpRelocaters = lpReloc->mpNext;
		}
		mMEMFREE( lpReloc );
	}
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_Find( sAsset * apAsset )
* ACTION   : RelocaterManager_Find
* CREATION : 08.12.2003 PNK
*-----------------------------------------------------------------------------------*/

sRelocater *	RelocaterManager_Find( sAsset * apAsset )
{
	sRelocater *	lpReloc;


	lpReloc = gpRelocaters;

	while( lpReloc )
	{
		if( lpReloc->mExtID == apAsset->mExtID )
		{
			DebugLog_Printf3( "RelocaterManager_Find(): %lX %lX %lX", lpReloc, lpReloc->mExtID, apAsset->mExtID );
			if( lpReloc->IsType( apAsset->mpData, apAsset->mSize, apAsset->mID ) )
			{
				return( lpReloc );
			}
		}
		lpReloc = lpReloc->mpNext;
	}

	return( lpReloc );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_DoInit( sAsset * apAsset )
* ACTION   : RelocaterManager_DoInit
* CREATION : 08.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	RelocaterManager_DoInit( sAsset * apAsset )
{
	sRelocater *	lpReloc;
	U32				lRet;

	DebugLog_Printf0( "RelocaterManager_DoInit()" );
	lRet    = 0;

	if( !apAsset->mInitFlag )
	{
		lpReloc = RelocaterManager_Find( apAsset );

		if( lpReloc )
		{
			if( lpReloc->DoInit )
			{
				lRet = lpReloc->DoInit( apAsset->mpData, apAsset->mSize, apAsset->mID );
				if( lRet )
				{
					apAsset->mInitFlag = 1;
				}
			}
		}
	}

	return( lRet );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_DoDeInit( sAsset * apAsset )
* ACTION   : RelocaterManager_DoDeInit
* CREATION : 08.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	RelocaterManager_DoDeInit( sAsset * apAsset )
{
	sRelocater *	lpReloc;
	U32				lRet;

	lRet    = 0;

	if( apAsset->mInitFlag )
	{
		lpReloc = RelocaterManager_Find( apAsset );

		if( lpReloc )
		{
			if( lpReloc->DoDeInit )
			{
				lRet = lpReloc->DoDeInit( apAsset->mpData, apAsset->mSize, apAsset->mID );
				if( lRet )
				{
					apAsset->mInitFlag = 0;
				}
			}
			else
			{
				apAsset->mInitFlag = 0;
			}
		}
		else
		{
			apAsset->mInitFlag = 0;
		}
	}

	return( lRet );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_DoDelocate( sAsset * apAsset )
* ACTION   : RelocaterManager_DoDelocate
* CREATION : 08.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	RelocaterManager_DoDelocate( sAsset * apAsset )
{
	sRelocater *	lpReloc;
	U32				lRet;

	lRet = 0;
	if( apAsset->mRelocFlag )
	{
		lpReloc = RelocaterManager_Find( apAsset );

		if( lpReloc )
		{
			if( lpReloc->DoDelocate )
			{
				lRet = lpReloc->DoDelocate( apAsset->mpData, apAsset->mSize, apAsset->mID );
				if( lRet )
				{
					apAsset->mRelocFlag = 0;
				}
			}
			else
			{
				apAsset->mRelocFlag = 0;
			}
		}
		else
		{
			apAsset->mRelocFlag = 0;
		}
	}
	return( lRet );
}


/*-----------------------------------------------------------------------------------*
* FUNCTION : RelocaterManager_DoRelocate( sAsset * apAsset )
* ACTION   : RelocaterManager_DoRelocate
* CREATION : 08.12.2003 PNK
*-----------------------------------------------------------------------------------*/

U32	RelocaterManager_DoRelocate( sAsset * apAsset )
{
	sRelocater *	lpReloc;
	U32				lRet;

	DebugLog_Printf0( "RelocaterManager_DoRelocate()" );
	lRet = 0;
	if( !apAsset->mRelocFlag )
	{
		lpReloc = RelocaterManager_Find( apAsset );

		if( lpReloc )
		{
			if( lpReloc->DoRelocate )
			{
				lRet = lpReloc->DoRelocate( apAsset->mpData, apAsset->mSize, apAsset->mID );
				if( lRet )
				{
					apAsset->mRelocFlag = 1;
				}
			}
		}
	}
	return( lRet );
}


/* ################################################################################ */
