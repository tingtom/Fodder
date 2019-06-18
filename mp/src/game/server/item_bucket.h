#pragma once
#include "cbase.h"
#include "props.h"

enum Drops
{
	Weapon,
	Money,
	Ammo,
	Nothing
};

#define MAX_MONEY 200
#define MAX_AMMO 50
#define WEAPON_RARITY 5
#define MODEL "models/props_junk/metalbucket01a.mdl"

class CItemBucket : public CPhysicsProp
{
	DECLARE_CLASS( CItemBucket, CPhysicsProp )

public:
	void Spawn();
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int ObjectCaps( void ) { return (BaseClass::ObjectCaps() | (FCAP_IMPULSE_USE|FCAP_USE_IN_RADIUS)); };

private:
	void Drop( CBasePlayer *pPlayer, Drops drop );
};

//
class CHealthBucket : public CItemBucket
{
	DECLARE_CLASS( CHealthBucket, CItemBucket )

public:
	void Spawn()
	{
		BaseClass::Spawn();

		int rem = random->RandomInt( 0, 2 );
		if( !rem )
			UTIL_Remove( this );
	}

	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		CBasePlayer *pPlayer = dynamic_cast< CBasePlayer* >( pActivator );
		if( !pPlayer )
			return;

		float healthToTake = random->RandomFloat( 0.0f, (float)pPlayer->GetHealth()-1.0f );
		float healthToGive = random->RandomFloat( 0.0f, (float)(pPlayer->GetHealth()/2)-1.0f );

		CTakeDamageInfo info;
		info.AddDamage( healthToTake );
		info.SetDamageType( DMG_GENERIC );

		pPlayer->TakeDamage( info );
		pPlayer->TakeHealth( healthToGive, DMG_GENERIC );
	}
};