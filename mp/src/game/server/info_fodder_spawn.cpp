#include "cbase.h"
#include "info_fodder_room.h"
#include "info_fodder_spawn.h"

//Spawning
#include "datacache/imdlcache.h"
#include "ai_basenpc.h"

LINK_ENTITY_TO_CLASS( info_fodder_spawn, CFodderSpawn );

BEGIN_DATADESC( CFodderSpawn )
	DEFINE_KEYFIELD( type, FIELD_INTEGER, "EnemyType" )
END_DATADESC()

const char* enemys[] = 
{
	"npc_combine",
	"npc_zombie",
	"npc_combine_s"
};

const char* enemyWeapons[] =
{
	"weapon_smg1",
	"weapon_shotgun"
};

void CFodderSpawn::SpawnEnemy()
{
	if( type == -1 )
		return;

	const char* enemyName = enemys[type];

	MDLCACHE_CRITICAL_SECTION();

	bool allowPrecache = CBaseEntity::IsPrecacheAllowed();
	CBaseEntity::SetAllowPrecache( true );

	//Create entity
	CAI_BaseNPC *baseNPC = dynamic_cast< CAI_BaseNPC * >( CreateEntityByName( enemyName ) );
	if( baseNPC )
	{
		baseNPC->Precache();

		int randomWep = random->RandomInt( 0, 1 );
		baseNPC->KeyValue( "additionalequipment", enemyWeapons[randomWep] );
		baseNPC->SetAbsOrigin( GetAbsOrigin() );
		baseNPC->SetName( GetEntityName() );
		baseNPC->SetOwnerEntity( parent );

		QAngle angles = GetAbsAngles();
		angles.x = 0.0;
		angles.z = 0.0;
		baseNPC->SetAbsAngles( angles );

		DispatchSpawn(baseNPC);
		baseNPC->Activate();
	}

	CBaseEntity::SetAllowPrecache( allowPrecache );
}

void CFodderSpawn::KillEnemys()
{
	CBaseEntity *entity = gEntList.FindEntityByClassname( NULL, enemys[type] );
	
	while( entity )
	{
		CAI_BaseNPC *baseNPC = dynamic_cast< CAI_BaseNPC* >( entity );

		if( baseNPC )
		{
			if( baseNPC->GetEntityName() == GetEntityName() )
			{
				if( baseNPC->GetActiveWeapon() )
					UTIL_Remove( baseNPC->GetActiveWeapon() );

				UTIL_Remove( baseNPC );
			}
		}

		entity = gEntList.FindEntityByClassname( entity, enemys[type] );
	}
}