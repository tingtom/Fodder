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
class CHudConsumables : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudConsumables, vgui::Panel);
 
public:
	CHudConsumables(const char * pElementName);
 
	virtual void Init (void);
	virtual void Reset (void);
	void MsgFunc_FloorData( bf_read &msg );
 
protected:
	virtual void Paint();
	virtual void PaintBackground();

private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Trebuchet24" );
};
 
DECLARE_HUDELEMENT( CHudConsumables );
DECLARE_HUD_MESSAGE( CHudConsumables, FloorData );

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudConsumables::CHudConsumables (const char * pElementName) : CHudElement (pElementName), BaseClass (NULL, "HudConsumables")
{
	vgui:: Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_WEAPONSELECTION );
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudConsumables::Init()
{
	HOOK_HUD_MESSAGE( CHudConsumables, FloorData );
	
	Reset();
}
 
//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
void CHudConsumables::Reset (void)
{
	SetPaintEnabled( true );
	SetPaintBackgroundEnabled( true );
	SetAlpha( 255 );
}
 
//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
void CHudConsumables::Paint()
{
	//text
	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( 255, 255, 255, 255 );
	
	int x = 0;
	for( int i = 0; i < 3; i++ )
	{
		surface()->DrawSetTextPos( x, 0 );

		wchar_t unicode[14];
		swprintf( unicode, L"%d", i );

		surface()->DrawUnicodeString( unicode );
		x += 50;
	}
}

void CHudConsumables::PaintBackground()
{
	SetBgColor( Color(0, 0, 0) );
	SetPaintBorderEnabled( false );

	BaseClass::PaintBackground();
}

void CHudConsumables::MsgFunc_FloorData( bf_read &msg )
{

}