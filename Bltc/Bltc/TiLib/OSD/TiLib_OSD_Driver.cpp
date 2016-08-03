// Author : Gamma Chen

#ifdef CONFIG_WITH_OSD

#include "OSD/TiLib_OSD_Driver.h"

#include "BhlGraphics.h"
#include "btypedef.h"
#include "bvdc.h"
#include "BhlVideoDisplay.h"

extern "C" {
#include "bCmdGfx.h"
}

#include "graphicsSurfDraw.h"

static tHalGraphicsSurf  gfxMap = { (SURFACETYPE)0,0,0,0,0 };

//##ModelId=48DC5D4603A0
bool TiLib_OSD_Driver::Init()
{
// Init a big GraphicsDisplaySurface to draw.
    if (gfxMap.uwWidth != 0) {
    	return false;
    }
	//enable argb4444 surface to both Component & Composite out
	ASSERT_ERR( (GI_NO_ERROR == GraphicsSurfaceCreate( HAL_GRAPHICS_MODE_ARGB8888, SCREEN_WIDTH, SCREEN_HEIGHT, &gfxMap)));

    ASSERT_ERR( (GI_NO_ERROR == GraphicsDisplayDestRectSet( HAL_GRAPHICS_DISPLAY0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
    ASSERT_ERR( (GI_NO_ERROR == GraphicsDisplayDestRectSet( HAL_GRAPHICS_DISPLAY1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));

    ASSERT_ERR( (GI_NO_ERROR == GraphicsDisplaySurfaceSet( HAL_GRAPHICS_DISPLAY_ALL, &gfxMap, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));

    GraphicsDisplaySurfaceEnableSet(HAL_GRAPHICS_DISPLAY0, TRUE);
    GraphicsDisplaySurfaceEnableSet(HAL_GRAPHICS_DISPLAY1, TRUE); 

    getSetOnScreenHandle(DO_SET, (PSURFACE) &gfxMap);    //set CURRENT_ONSCREEN to newSurf_FrontSurf
    //init a pattern in the buffer:
    
    ClearScreen();
	
    return true;
    
ERR_EXIT: 
    return false;
}

//##ModelId=48DC5D4603B0
void TiLib_OSD_Driver::ClearScreen()
{
    GFX_RECT_FILL( CURRENT_ONSCREEN, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		
	return;
}

//##ModelId=48DC5D4603C0
void TiLib_OSD_Driver::Test()
{
    UWORD displaySurfWidth = SCREEN_WIDTH;
    UWORD displaySurfHeight = 100;
    UWORD displaySurf_X = 0;
    UWORD displaySurf_Y = SCREEN_HEIGHT - displaySurfHeight;
    
    FRAME(displaySurf_X, displaySurf_Y, displaySurfWidth-1, displaySurf_Y+displaySurfHeight-1, 10, 0xFF0000FF);    
    FRAME(displaySurf_X+10, displaySurf_Y+10, displaySurfWidth-1-10, displaySurf_Y+displaySurfHeight-1-10, 10, 0xFF00FF00);    
    FRAME(displaySurf_X+20, displaySurf_Y+20, displaySurfWidth-1-20, displaySurf_Y+displaySurfHeight-1-20, 10, 0xFFFF0000);    
    FRAME(displaySurf_X+30, displaySurf_Y+30, displaySurfWidth-1-30, displaySurf_Y+displaySurfHeight-1-30, 10, 0xFFFF00FF);    
    FRAME(displaySurf_X+40, displaySurf_Y+40, displaySurfWidth-1-40, displaySurf_Y+displaySurfHeight-1-40, 10, 0xFFFFFF00);    
}

//##ModelId=48DC5D470007
void TiLib_OSD_Driver::SetPoint(U16 x, U16 y, U32 color)
{
//    GFX_RECT_FILL( CURRENT_ONSCREEN, x, y, 1, 1, color); // very slow
	GraphicsPixelSet(CURRENT_ONSCREEN, x, y, color);
}

/*
// Don't use this function since GFX_RECT_FILL() will confict with Timer Interrupt Mechanism, TI003.
void TiLib_OSD_Driver::DrawRect(TiLib_Rect *rect)
{
    GFX_RECT_FILL( CURRENT_ONSCREEN, rect->x, rect->y, rect->w, rect->h, rect->color);
}
*/

#endif	// CONFIG_WITH_OSD

