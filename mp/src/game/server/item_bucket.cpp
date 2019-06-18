#include "cbase.h"
#include "item_bucket.h"
#include "ammodef.h"

LINK_ENTITY_TO_CLASS( item_bucket, CItemBucket );
LINK_ENTITY_TO_CLASS( item_bucket_health, CHealthBucket );

const char *drops[] =
{
	"weapon_smg1",
	"weapon_shotgun",
	"weapon_crossbow"
};

void CItemBucket::Spawn()
{
	SetModelName( AllocPooledString( MODEL ) );
	Precache();

	SetModel( MODEL );
	SetSolid( SOLID_VPHYSICS );
	BaseClass::Spawn();
}

void CItemBucket::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer = dynamic_cast< CBasePlayer* >( pActivator );
	if( !pPlayer )
		return;

	bool drop = false;
	while( !drop )
	{
		int randomInt = random->RandomInt( 0, Nothing );

		//Weapons are rarer
		if( randomInt == Weapon )
		{
			randomInt = random->RandomInt( 0, WEAPON_RARITY );
			if( randomInt == 0 )
				Drop( pPlayer, Weapon );
			else
				continue;
		}else
			Drop( pPlayer, (Drops)randomInt );

		drop = true;
	}

	UTIL_Remove( this );
}

void CItemBucket::Drop( CBasePlayer *pPlayer, Drops drop )
{
	switch( drop )
	{
	case Weapon:
		{
			int randomWeapon = random->RandomInt( 0, 2 );
			Msg( "RandomWeapon: %i\n", randomWeapon );
			pPlayer->GiveNamedItem( drops[randomWeapon] );
			break;
		}

	case Money:
		{
			int randomHealth = random->RandomInt( 0, MAX_MONEY );
			Msg( "RandomHealth: %i\n", randomHealth );
			pPlayer->TakeHealth( randomHealth, DMG_GENERIC );
			break;
		}

	case Ammo:
		{
			int randomAmmo = random->RandomInt( 0, MAX_AMMO );
			Msg( "RandomAmmo: %i\n", randomAmmo );

			if( pPlayer->GetActiveWeapon() )
			{
				CBaseCombatWeapon *weapon = pPlayer->GetActiveWeapon();
				
				//Already full
				if( weapon->m_iClip1 == weapon->GetMaxClip1() )
				{
					//Ammo count is maxed out so give secondary ammo
					if( pPlayer->GetAmmoCount( weapon->m_iPrimaryAmmoType ) == GetAmmoDef()->MaxCarry( weapon->m_iPrimaryAmmoType ) )
					{
						if( weapon->UsesSecondaryAmmo() )
						{
							if( weapon->UsesClipsForAmmo2() )
							{
								if( weapon->m_iClip2 + 1 < weapon->GetMaxClip2() )
									weapon->m_iClip2 += 1;
							}else{
								pPlayer->GiveAmmo( 1, weapon->m_iSecondaryAmmoType );
							}
						}

						return;
					}

					//Give ammo to ammo count
					if( pPlayer->GetAmmoCount( weapon->m_iPrimaryAmmoType ) + randomAmmo >= GetAmmoDef()->MaxCarry( weapon->m_iPrimaryAmmoType ) )
						pPlayer->GiveAmmo( GetAmmoDef()->MaxCarry( weapon->m_iPrimaryAmmoType ), weapon->m_iPrimaryAmmoType );
					else
						pPlayer->GiveAmmo( randomAmmo, weapon->m_iPrimaryAmmoType );

					return;
				}

				//Cant add the ammo
				if( weapon->m_iClip1 + randomAmmo >= weapon->GetMaxClip1() )
				{
					weapon->m_iClip1 = weapon->GetMaxClip1();
				}else
					weapon->m_iClip1 += randomAmmo;
			}

			break;
		}
	}
}