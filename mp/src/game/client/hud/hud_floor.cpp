#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "iclientmode.h"
#include "c_basehlplayer.h"
#include "vgui_controls/Panel.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/ISurface.h"
#include <vgui/ILocalize.h>
 
using namespace vgui;
 
#include "tier0/memdbgon.h" 

//-----------------------------------------------------------------------------
// Purpose: Shows the round number
//-----------------------------------------------------------------------------
class CHudFloor : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudFloor, vgui::Panel);
 
public:
	CHudFloor(const char * pElementName);
 
	virtual void Init (void);
	virtual void Reset (void);
	void MsgFunc_FloorData( bf_read &msg );
 
protected:
	virtual void Paint();
	virtual void PaintBackground();

private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Trebuchet24" );
	int numFloor;
	int numRoom;
};
 
DECLARE_HUDELEMENT( CHudFloor );
DECLARE_HUD_MESSAGE( CHudFloor, FloorData );

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudFloor::CHudFloor (const char * pElementName) : CHudElement (pElementName), BaseClass (NULL, "HudFloor")
{
	vgui:: Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_WEAPONSELECTION );
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudFloor::Init()
{
	HOOK_HUD_MESSAGE( CHudFloor, FloorData );
	
	Reset();
}
 
//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
void CHudFloor::Reset (void)
{
	SetPaintEnabled( true );
	SetPaintBackgroundEnabled( true );
	SetAlpha( 255 );

	numFloor = -1;
	numRoom = -1;
}
 
//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
void CHudFloor::Paint()
{
	//text
	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( 255, 255, 255, 255 );
	surface()->DrawSetTextPos( 0, 0 );

	wchar_t unicode[14];
	swprintf( unicode, L"%d-%d", numFloor, numRoom );

	surface()->DrawUnicodeString( unicode );
}

void CHudFloor::PaintBackground()
{
	SetBgColor( Color(0, 0, 0) );
	SetPaintBorderEnabled( false );

	BaseClass::PaintBackground();
}

void CHudFloor::MsgFunc_FloorData( bf_read &msg )
{
	numFloor = msg.ReadByte();
	numRoom = msg.ReadByte();
}