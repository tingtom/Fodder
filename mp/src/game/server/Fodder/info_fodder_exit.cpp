#include "cbase.h"
#include "info_fodder_exit.h"
#include "info_fodder_room.h"

#include "hl2_player.h"
#include "hl2_gamerules.h"

LINK_ENTITY_TO_CLASS( info_fodder_exit, CFodderExit );

BEGIN_DATADESC( CFodderExit )
	DEFINE_INPUTFUNC( FIELD_VOID, "Teleport", InputTeleport )
END_DATADESC()

void CFodderExit::InputTeleport( inputdata_t &inputdata )
{
	CHL2_Player *pPlayer = dynamic_cast< CHL2_Player* >( inputdata.pActivator );

	if( pPlayer && parent->ExitAllowed() )
	{
		//Were teleporting from the exit room
		if( parent->GetRoomType() == Exit )
		{
			CFodderWorld::GetCFodderWorld()->GetRandomLevel();
			return;
		}

		CFodderRoom *newRoom = CFodderWorld::GetCFodderWorld()->GetRandomRoom( parent->GetRoomType() );

		//Randomly select a exit but only if its activated
		CFodderExit *newExit = NULL;

		do
		{
			int randomExit = random->RandomInt( 0, newRoom->GetNumExits()-1 );
			newExit = newRoom->GetExit( randomExit );
		}while( !newExit );
		
		if( newExit )
		{
			//Do these before the teleport so seeing enemys spawn is less likely
			parent->PlayerExited();
			newExit->GetRoomParent()->PlayerEntered();

			pPlayer->SetLocalOrigin( newExit->GetAbsOrigin() + Vector(0,0,1) );
			pPlayer->SetAbsVelocity( vec3_origin );
			pPlayer->SetLocalAngles( newExit->GetLocalAngles() );
			pPlayer->m_Local.m_vecPunchAngle = vec3_angle;
			pPlayer->m_Local.m_vecPunchAngleVel = vec3_angle;
			pPlayer->SnapEyeAngles( newExit->GetLocalAngles() );
		}
	}

	if( parent->DisableOnUse() )
	{
		parent->SetActivated( false );
	}
}