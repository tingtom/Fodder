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
class CHudHealth : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudHealth, vgui::Panel);
 
public:
	CHudHealth(const char * pElementName);
 
	virtual void Init (void);
	virtual void Reset (void);
	virtual void OnThink (void);
 
protected:
	virtual void Paint();
	virtual void PaintBackground();

private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "Trebuchet24" );
	int health;
};
 
DECLARE_HUDELEMENT( CHudHealth );

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------
CHudHealth::CHudHealth (const char * pElementName) : CHudElement (pElementName), BaseClass (NULL, "HudHealth")
{
	vgui:: Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_WEAPONSELECTION );
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudHealth::Init()
{
	Reset();
}
 
//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------
void CHudHealth::Reset (void)
{
	SetPaintEnabled( true );
	SetPaintBackgroundEnabled( true );
	SetAlpha( 255 );

	health = 0;
}
 
//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------
void CHudHealth::OnThink (void)
{
	CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
	if( pPlayer )
	{
		health = pPlayer->GetHealth();
	}
}
 
//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------
void CHudHealth::Paint()
{
	//text
	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( 255, 255, 255, 255 );
	surface()->DrawSetTextPos( 0, 0 );

	wchar_t unicode[25];
	swprintf( unicode, L"%d", health );

	surface()->DrawUnicodeString( unicode );
}

void CHudHealth::PaintBackground()
{
	SetBgColor( Color(0, 0, 0) );
	SetPaintBorderEnabled( false );

	BaseClass::PaintBackground();
}