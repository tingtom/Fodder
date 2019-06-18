#pragma once
#include "cbase.h"

class CFodderRoom;
class CFodderSpawn : public CPointEntity
{
public:
	DECLARE_CLASS( CFodderSpawn, CPointEntity );
	DECLARE_DATADESC();

	CFodderSpawn()
	{
		type = -1;
	}

	void SpawnEnemy();
	void KillEnemys();

	void SetRoomParent( CFodderRoom *parent )
	{
		this->parent = parent;
	}

	CFodderRoom *GetRoomParent()
	{
		return parent;
	}

private:
	int type;
	CFodderRoom *parent;
};