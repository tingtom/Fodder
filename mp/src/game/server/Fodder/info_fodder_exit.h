#pragma once
#include "cbase.h"

class CFodderRoom;
class CFodderExit : public CPointEntity
{
public:
	DECLARE_CLASS( CFodderExit, CPointEntity );
	DECLARE_DATADESC();

	CFodderExit()
	{
	}

	void SetRoomParent( CFodderRoom *parent )
	{
		this->parent = parent;
	}

	CFodderRoom *GetRoomParent()
	{
		return parent;
	}

private:
	void InputTeleport( inputdata_t &inputdata );
	CFodderRoom *parent;
};
