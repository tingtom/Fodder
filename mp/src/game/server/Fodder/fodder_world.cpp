#include "cbase.h"
#include "fodder_world.h"
#include "FileSystem.h"
#include "fmtstr.h"

CFodderWorld* CFodderWorld::instance = NULL;

void CFodderWorld::InitCFodderWorld( int i )
{
	if( !instance )
	{
		instance = new CFodderWorld( i );
	}
}

CFodderWorld* CFodderWorld::GetCFodderWorld()
{
	return instance;
}

void CFodderWorld::ReleaseCFodderWorld()
{
	delete instance;
}

bool CFodderWorld::GetRooms()
{
	CBaseEntity *entity = gEntList.FindEntityByClassname( NULL, "info_fodder_room" );
	bool spawnExists = false;
	bool exitExists = false;

	while( entity )
	{
		CFodderRoom *fodderRoom = dynamic_cast< CFodderRoom* >( entity );

		if( fodderRoom )
		{
			spawnExists = fodderRoom->CanSpawn();
			exitExists = ( fodderRoom->GetRoomType() == Exit );
			rooms.AddToTail( fodderRoom );
		}

		entity = gEntList.FindEntityByClassname( entity, "info_fodder_room" );
	}

	if( rooms.Count() == 0 )
		return false;

	//While getting rooms a spawn wasn't found so set the first room to the spawn
	if( !spawnExists )
	{
		rooms[0]->SetCanSpawn( true );
		Msg( "FodderWorld: No spawn found, choosing first room\n" );
	}

	if( !exitExists )
	{
		rooms[rooms.Count()-1]->SetRoomType( Exit );
		Msg( "FodderWorld: No exit found, choosing last room\n" );
	}

	return true;
}

bool CFodderWorld::GetRandomLevel()
{
	FileFindHandle_t findHandle;
	const char *pszFilename = g_pFullFileSystem->FindFirst( "maps/*.bsp", &findHandle );
	const char* maps[100];
	int index = 0;

	while ( pszFilename )
	{
		char mapname[256];

		// FindFirst ignores the pszPathID, so check it here
		// TODO: this doesn't find maps in fallback dirs
		Q_snprintf( mapname, sizeof(mapname), "maps/%s", pszFilename );
		if ( !g_pFullFileSystem->FileExists( mapname, "MOD" ) )
		{
			goto nextFile;
		}

		// remove the text 'maps/' and '.bsp' from the file name to get the map name
		const char *str = Q_strstr( pszFilename, "maps" );
		if ( str )
		{
			Q_strncpy( mapname, str + 5, sizeof(mapname) - 1 );	// maps + \\ = 5
		}
		else
		{
			Q_strncpy( mapname, pszFilename, sizeof(mapname) - 1 );
		}

		char *ext = Q_strstr( mapname, ".bsp" );
		if ( ext )
		{
			*ext = 0;
		}

		maps[index] = mapname;
		index++;

		// get the next file
	nextFile:
		pszFilename = g_pFullFileSystem->FindNext( findHandle );
	}
	g_pFullFileSystem->FindClose( findHandle );

	//Save the players data
	Save();
	rooms.RemoveAll();
	numRooms = 1;
	numFloor++;

	if( index == 1 )
	{
		engine->ChangeLevel( maps[0], NULL );
		Msg( "FodderWorld: Cannot find a new level, choosing same level\n" );
		return false;
	}

	bool goodMap = true;
	int mapIndex = 0;

	do
	{
		//Choose a random map
		mapIndex = random->RandomInt( 0, index );

		//Is this map the current one?
		if( strcmp( maps[mapIndex], STRING( gpGlobals->mapname ) ) == 0 )
		{
			goodMap = false;
		}

	}while( !goodMap );

	engine->ChangeLevel( maps[mapIndex], NULL );
	Msg( "FodderWorld: Loading level %s\n", maps[mapIndex] );

	SendFloorData();
	return true;
}

CFodderRoom* CFodderWorld::GetRandomRoom( RoomType type )
{
	//Randomly choose a room
	int randomRoom = 0;
	bool isRoomOk = false;

	do
	{
		randomRoom = random->RandomInt( 0, rooms.Count()-1 );

		CFodderRoom *room = rooms[ randomRoom ];
		isRoomOk = true;

		//Room doesnt want to be tped to
		if( !room->IsActivated() )
			isRoomOk = false;

		//If the random room is a loot room randomly choose to keep it
		if( room->GetRoomType() == LootCorridor || room->GetRoomType() == LootRoom )
		{
			int randomLoot = random->RandomInt( 0, 5 );
			if( randomLoot == 0 )
				isRoomOk = false;

			if( type == LootCorridor || type == LootRoom )
				isRoomOk = false;
		}

		//Make sure its the exit room
		if( numRooms >= maxRooms )
		{
			room->GetRoomType() == Exit ? isRoomOk = true : isRoomOk = false;
		}else if ( room->GetRoomType() == Exit )
			isRoomOk = false;
	}
	while( !isRoomOk );
	//Keep looping until a specific room is found

	numRooms++;
	SendFloorData();
	return rooms[randomRoom];
}