#include "cbase.h"
#include "info_fodder_room.h"
#include "item_bucket.h"

LINK_ENTITY_TO_CLASS( info_fodder_room, CFodderRoom );

BEGIN_DATADESC( CFodderRoom )
	DEFINE_KEYFIELD( canSpawn, FIELD_BOOLEAN, "CanSpawn" ),
	DEFINE_KEYFIELD( disableOnUse, FIELD_BOOLEAN, "disableOnUse" ),
	DEFINE_KEYFIELD( type, FIELD_INTEGER, "RoomType" ),

	DEFINE_KEYFIELD( exitNames[0], FIELD_STRING, "ExitOne" ),
	DEFINE_KEYFIELD( exitNames[1], FIELD_STRING, "ExitTwo" ),
	DEFINE_KEYFIELD( exitNames[2], FIELD_STRING, "ExitThree" ),
	DEFINE_KEYFIELD( exitNames[3], FIELD_STRING, "ExitFour" ),

	DEFINE_KEYFIELD( spawnerName, FIELD_STRING, "SpawnerName" ),
	DEFINE_KEYFIELD( maxSpawns, FIELD_INTEGER, "maxSpawns" )
END_DATADESC()

CFodderRoom::CFodderRoom()
{
	canSpawn = false;
	type = None;
	getEntitys = false;
	activated = true;
	numExits = 0;

	maxSpawns = 5;
	enemysKilled = 0;
	allowExit = true;

	exits[0] = NULL;
	exits[1] = NULL;
	exits[2] = NULL;
	exits[3] = NULL;

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CFodderRoom::PlayerEntered()
{
	if( !spawners.Count() )
	{
		if( !getEntitys )
		{
			//Try and get the spawners and try again
			Think();
			PlayerEntered();
		}

		return;
	}

	if( maxSpawns > spawners.Count() )
		maxSpawns = spawners.Count();

	int max = maxSpawns;
	CUtlVector<int> indexs;

	do
	{
		int randomSpawn = random->RandomInt( 0, spawners.Count()-1 );
		if( indexs.HasElement( randomSpawn ) )
			continue;

		spawners[randomSpawn]->SpawnEnemy();
		indexs.AddToTail( randomSpawn );
		max--;
	}while( max );

	indexs.RemoveAll();
}

void CFodderRoom::PlayerExited()
{
	if( !spawners.Count() )
		return;

	for( int i = 0; i < spawners.Count(); i++ )
		spawners[i]->KillEnemys();

	allowExit = false;
	enemysKilled = 0;
}

void CFodderRoom::DeathNotice( CBaseEntity *pVictim )
{	
	enemysKilled++;

	if( !allowExit )
	{
		if( enemysKilled >= maxSpawns )
		{
			allowExit = true;

			CItemBucket *bucket = dynamic_cast< CItemBucket* >( CreateEntityByName( "item_bucket" ) );
			bucket->SetAbsOrigin( GetAbsOrigin() );
			bucket->SetAbsAngles( QAngle( 0, 0, 0 ) );

			DispatchSpawn( bucket );
		}
	}
}

void CFodderRoom::Think()
{
	//Convert the names from hammer into entitys
	if( !getEntitys )
	{
		for( int i = 0; i < 4; i++ )
		{
			const char* exitName = STRING(exitNames[i]);

			if( strlen( exitName ) && !exits[i] )
			{
				exits[i] = dynamic_cast< CFodderExit* >( gEntList.FindEntityByName( NULL, exitName ) );
				exits[i]->SetRoomParent( this );
				numExits++;
			}
		}

		//Get all spawners of this name and store them
		CBaseEntity *ent = gEntList.FindEntityByName( NULL, STRING( spawnerName ) );

		while( ent )
		{
			CFodderSpawn *spawn = dynamic_cast< CFodderSpawn* >( ent );
			spawn->SetRoomParent( this );
			spawners.AddToTail( spawn );

			//Player cant leave until all enemys killed
			allowExit = false;

			ent = gEntList.FindEntityByName( ent, STRING( spawnerName ) );
		}

		getEntitys = true;
	}
}