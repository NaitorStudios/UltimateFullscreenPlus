// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "common.h"


// ============================================================================
//
//Functions
//
// ============================================================================

int GetScreenWidth(LPRH rhPtr)
{
    //HDC winMonitor = GetDC(rhPtr->rhHMainWin);
    //return GetDeviceCaps(winMonitor, HORZRES);

    HMONITOR monitor = MonitorFromWindow(rhPtr->rhHMainWin, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    return info.rcMonitor.right - info.rcMonitor.left;

    //return GetSystemMetrics(SM_CXSCREEN);
}

int GetScreenHeight(LPRH rhPtr)
{
    //HDC winMonitor = GetDC(rhPtr->rhHMainWin);
    //return GetDeviceCaps(winMonitor, VERTRES);

    HMONITOR monitor = MonitorFromWindow(rhPtr->rhHMainWin, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    return info.rcMonitor.bottom - info.rcMonitor.top;

    //return GetSystemMetrics(SM_CXSCREEN);
}

inline int GetGameWidth(LPRH rhPtr)
{
  return rhPtr->rh3.rh3WindowSx;
}

inline int GetGameHeight(LPRH rhPtr)
{
  return rhPtr->rh3.rh3WindowSy;
}

void ScaleRect(RECT *ret, float scale, LPRH rhPtr)
{
  int xmiddle = GetScreenWidth(rhPtr)/2;
  int ymiddle = GetScreenHeight(rhPtr)/2;

  ret->left   -= xmiddle; 
  ret->right  -= xmiddle;
  ret->top    -= ymiddle;
  ret->bottom -= ymiddle;

  ret->left   = (long)(ret->left  *scale);
  ret->right  = (long)(ret->right *scale);
  ret->top    = (long)(ret->top   *scale);
  ret->bottom = (long)(ret->bottom*scale);

  ret->left   += xmiddle;
  ret->right  += xmiddle;
  ret->top    += ymiddle;
  ret->bottom += ymiddle;
}

//Calculate where the game rect should be based on what sizing method has been chosen.
RECT CalculateGameRectangle(LPRDATA rdPtr)
{
  RECT ret{};
  LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;

  /*if ((ret.right - ret.left) <= GetScreenWidth(rhPtr))
      GlobalData.AppWidth = GetGameWidth(rdPtr->rHo.hoAdRunHeader);
  else
      GlobalData.AppWidth = (GetGameWidth(rdPtr->rHo.hoAdRunHeader) * GetScreenWidth(rhPtr)) / (ret.right - ret.left);

  if ((ret.bottom - ret.top) <= GetScreenHeight(rhPtr))
      GlobalData.AppHeight = GetGameHeight(rdPtr->rHo.hoAdRunHeader);
  else
      GlobalData.AppHeight = (GetGameHeight(rdPtr->rHo.hoAdRunHeader) * GetScreenHeight(rhPtr)) / (ret.bottom - ret.top);
  */

  float game_aspect = GetGameWidth(rhPtr)/(float)GetGameHeight(rhPtr);
  float screen_aspect = GetScreenWidth(rhPtr)/(float)GetScreenHeight(rhPtr);

  switch(GlobalData.fstype)
  {
    default:
    case MANUAL:
      ret.left = GlobalData.gamerectx;
      ret.top  = GlobalData.gamerecty;
      ret.right = GlobalData.gamerectwidth + GlobalData.gamerectx;
      ret.bottom = GlobalData.gamerectheight + GlobalData.gamerecty;
      break;

    case BESTFIT:
      if(screen_aspect > game_aspect) //screen is too wide
      {
        //Size the height exactly to the screen, then size the width proportionately
        ret.top    = 0;
        ret.bottom = GetScreenHeight(rhPtr);
        int width  = (GetScreenHeight(rhPtr) * GetGameWidth(rhPtr))/GetGameHeight(rhPtr);
        ret.left   = GetScreenWidth(rhPtr)/2 - width/2;
        ret.right  = GetScreenWidth(rhPtr)/2 + width/2;
      }
      else //screen is too tall
      {
        //Size the width to the screen, then size the height
        ret.left   = 0;
        ret.right  = GetScreenWidth(rhPtr);
        int height = (GetScreenWidth(rhPtr) * GetGameHeight(rhPtr))/GetGameWidth(rhPtr);
        ret.top    = GetScreenHeight(rhPtr)/2 - height/2;
        ret.bottom = GetScreenHeight(rhPtr)/2 + height/2;
      }
      break;

    case BESTPIXELPERFECTFIT:
      if(screen_aspect > game_aspect) //screen is too wide
      {
        int scale  = GetScreenHeight(rhPtr)/GetGameHeight(rhPtr);   //calculate the highest possible integer scale
        int width  = GetGameWidth(rhPtr)*scale;  //calculate width and height based on this scale
        int height = GetGameHeight(rhPtr)*scale;
        ret.top    = GetScreenHeight(rhPtr)/2 - height/2;  //place rectangle
        ret.bottom = GetScreenHeight(rhPtr)/2 + height/2;
        ret.left   = GetScreenWidth(rhPtr) /2 - width /2;
        ret.right  = GetScreenWidth(rhPtr) /2 + width /2;
      }
      else //screen is too tall
      {
        int scale  = GetScreenWidth(rhPtr)/GetGameWidth(rhPtr);
        int width  = GetGameWidth(rhPtr)*scale;
        int height = GetGameHeight(rhPtr)*scale;
        ret.top    = GetScreenHeight(rhPtr)/2 - height/2;
        ret.bottom = GetScreenHeight(rhPtr)/2 + height/2;
        ret.left   = GetScreenWidth(rhPtr) /2 - width /2;
        ret.right  = GetScreenWidth(rhPtr) /2 + width /2;
      }
      break;

    case MATCHWIDTH:
    {
      //Size the width to the screen, then size the height
      ret.left   = 0;
      ret.right  = GetScreenWidth(rhPtr);
      int height = (GetScreenWidth(rhPtr) * GetGameHeight(rhPtr))/GetGameWidth(rhPtr);
      ret.top    = GetScreenHeight(rhPtr)/2 - height/2;
      ret.bottom = GetScreenHeight(rhPtr)/2 + height/2;
      break;
    }

    case MATCHHEIGHT:
    {
      //Size the height exactly to the screen, then size the width proportionately
      ret.top    = 0;
      ret.bottom = GetScreenHeight(rhPtr);
      int width  = (GetScreenHeight(rhPtr) * GetGameWidth(rhPtr))/GetGameHeight(rhPtr);
      ret.left   = GetScreenWidth(rhPtr)/2 - width/2;
      ret.right  = GetScreenWidth(rhPtr)/2 + width/2;
      break;
    }

    case ONETOONE:
    {
      ret.left   = GetScreenWidth(rhPtr) /2 - GetGameWidth(rhPtr) /2;
      ret.top    = GetScreenHeight(rhPtr)/2 - GetGameHeight(rhPtr)/2;
      ret.right  = GetScreenWidth(rhPtr) /2 + GetGameWidth(rhPtr) /2;
      ret.bottom = GetScreenHeight(rhPtr)/2 + GetGameHeight(rhPtr)/2;
      break;
    }

    case RESIZEWITHBORDER:
    {
        //Need to comment later
        //int FrameX = GlobalData.AppWidth;
        //int FrameY = GlobalData.AppHeight;
        int FrameX = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCxWin;
        int FrameY = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCyWin;
        
        int PlayX = GlobalData.PlayX;
        int PlayY = GlobalData.PlayY;

        int ClientWidth = 0;
        int ClientHeight = 0;
        RECT ClientRect;        
        if (GetClientRect(rhPtr->rhHMainWin, &ClientRect))
        {
            ClientWidth = ClientRect.right - ClientRect.left;
            ClientHeight = ClientRect.bottom - ClientRect.top;
        }
        
        GlobalData.Highest = min(ClientWidth / PlayX, ClientHeight / PlayY);
        GlobalData.OffsetX = (ClientWidth - PlayX * GlobalData.Highest) / 2 - (FrameX - PlayX) / 2 * GlobalData.Highest;
        GlobalData.OffsetY = (ClientHeight - PlayY * GlobalData.Highest) / 2 - (FrameY - PlayY) / 2 * GlobalData.Highest;

        ret.left = GlobalData.OffsetX;
        ret.right = GlobalData.OffsetX + FrameX * GlobalData.Highest;
        ret.top = GlobalData.OffsetY;
        ret.bottom = GlobalData.OffsetY + FrameY * GlobalData.Highest;
        break;
    }
  }

  ScaleRect(&ret, GlobalData.gamescale, rhPtr);
  return ret;
}


//This function is used whenever we do something that could require us to change the game rectangle's size
void UpdateGameRectangleSize(LPRDATA rdPtr)
{
  RECT gamerect = CalculateGameRectangle(rdPtr);

  LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;  //runheader pointer will allow us to get the handle to the game subwindow.
  HWND mainwindow = rhPtr->rhHMainWin;
  HWND subwindow = rhPtr->rhHEditWin;
  //HWND scrollingwindow = rhPtr->rhH
  if(GlobalData.fullscreen)
  {
    SetWindowPos
    (
      subwindow,
      HWND_TOP,
      gamerect.left,
      gamerect.top,
      gamerect.right  - gamerect.left,
      gamerect.bottom - gamerect.top,
      SWP_NOOWNERZORDER | SWP_NOZORDER
    );
  }
  else if(GlobalData.apply_fs_settings_to_window)   //it's windowed, but we're supposed to size it accordingly anyway
  {
    SetWindowPos
    (
      mainwindow,
      HWND_TOP,
      gamerect.left,
      gamerect.top,
      gamerect.right  - gamerect.left,
      gamerect.bottom - gamerect.top,
      SWP_NOOWNERZORDER | SWP_NOZORDER
    );

    //Set the subwindow to be in the normal position relative to the main window
    SetWindowPos
    (
      subwindow,
      HWND_TOP,
      0,
      0,
      gamerect.right  - gamerect.left,
      gamerect.bottom - gamerect.top,
      SWP_NOOWNERZORDER | SWP_NOZORDER
    );

  }
  else    //windowed; we just need to reset the subwindow's position
  {
    RECT winrect;
    GetWindowRect(subwindow, &winrect);

    SetWindowPos
    (
      subwindow,
      HWND_TOP,
      0,
      0,
      winrect.right  - winrect.left,
      winrect.bottom - winrect.top ,
      SWP_NOOWNERZORDER | SWP_NOZORDER
    );
  }
}


// ============================================================================
//
// CONDITIONS
// 
// ============================================================================


//IsFullscreen
CONDITION
(
	/* ID */			  0,
	/* Name */			"%o: Is fullscreen",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) 
{
	return GlobalData.fullscreen;
}

//IsWindowed
CONDITION
(
	/* ID */			  1,
	/* Name */			"%o: Is windowed",
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
)
{
	return !GlobalData.fullscreen;
}

//OnFullscreen
CONDITION
(
	/* ID */			  2,
	/* Name */			"%o: On fullscreen",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return true;
}

//OnWindowed
CONDITION
(
	/* ID */			  3,
	/* Name */			"%o: On windowed",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return true;
}

//OnChangeWindowMode
CONDITION
(
	/* ID */			  4,
	/* Name */			"%o: On change window mode",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return true;
}

//OnChangeWindowMode
CONDITION
(
    /* ID */			  5,
    /* Name */			"%o: On monitor switch",
    /* Flags */			0,
    /* Params */		(0)
)
{
    return true;
}


// ============================================================================
//
// ACTIONS
// 
// ============================================================================

//GoFullscreen
ACTION
(
	/* ID */			0,
	/* Name */			"Go fullscreen",
	/* Flags */			0,
	/* Params */		(0)
)
{
  //guard against multiple event triggers
  if(GlobalData.fullscreen)
    return;

  GlobalData.fullscreen = true;

  //Put "maximizing" code here
  LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
  HWND mainwin = rhPtr->rhHMainWin;
  if(!IsZoomed(mainwin))
    ShowWindow(mainwin, SW_MAXIMIZE);

  UpdateGameRectangleSize(rdPtr);

  rdPtr->rRd->GenerateEvent(2);  //trigger "on fullscreen" event
  rdPtr->rRd->GenerateEvent(4);  //trigger "on change window mode" event
}

//GoWindowed
ACTION
(
	/* ID */			  1,
	/* Name */			"Go windowed",
	/* Flags */			0,
	/* Params */		(0)
)
{
  //guard against multiple event triggers
  if(!GlobalData.fullscreen)
    return;

  GlobalData.fullscreen = false;

  //Put "restoring" code here
  LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
  HWND mainwin = rhPtr->rhHMainWin;
  if(IsZoomed(mainwin))
    ShowWindow(mainwin, SW_RESTORE);

  UpdateGameRectangleSize(rdPtr);

  rdPtr->rRd->GenerateEvent(3);  //trigger "on windowed" event
  rdPtr->rRd->GenerateEvent(4);  //trigger "on change window mode" event


}


//ApplyFullscreenSettingsToWindowedSize
ACTION
(
	/* ID */			  2,
	/* Name */			"Apply fullscreen settings to windowed size",
	/* Flags */			0,
	/* Params */		(0)
)
{
  //guard against multiple event triggers
  if(GlobalData.apply_fs_settings_to_window)
    return;

  if(!GlobalData.apply_fs_settings_to_window)
  {
    GlobalData.apply_fs_settings_to_window = true;
    if(!GlobalData.fullscreen)
    {
      //Resize the window right away
      UpdateGameRectangleSize(rdPtr);
    }
  }
}

//DontApplyFullscreenSettingsToWindowedSize
ACTION
(
	/* ID */			  3,
	/* Name */			"Don't apply fullscreen settings to windowed size",
	/* Flags */			0,
	/* Params */		(0)
)
{
    //guard against multiple event triggers
    if(!GlobalData.apply_fs_settings_to_window)
      return;

    GlobalData.apply_fs_settings_to_window = false;
}



//SetFullscreenType
ACTION
(
	/* ID */			  4,
	/* Name */			"Set fullscreen type to %0",
	/* Flags */			0,
	/* Params */		(1, PARAM_NUMBER, "Fullscreen type: 0=Best fit, 1=Best pixel perfect fit (integer-only scale), 2=One-to-one, 3=Match width(vert-), 4=Match height(hor+), 5=Manual settings, 6=Resize with Border")
)
{
  FULLSCREENTYPE param = (FULLSCREENTYPE)Param(TYPE_INT);

  //guard against multiple event triggers
  if(GlobalData.fstype == param)
    return;

  GlobalData.fstype = param;

  UpdateGameRectangleSize(rdPtr);
}

//SetGameScale
ACTION
(
	/* ID */			  5,
	/* Name */			"Set game scale to %0",
	/* Flags */			0,
	/* Params */		(1, PARAM_NUMBER, "Game scale (1.0=default)")
)
{
  long tmpf = Param(TYPE_FLOAT);
  float param = *(float*)&tmpf;

  //guard against multiple event triggers
  if(GlobalData.gamescale == param)
    return;

  GlobalData.gamescale = param;

  UpdateGameRectangleSize(rdPtr);
}


//SetManualFullscreenPosition
ACTION
(
	/* ID */			  6,
	/* Name */			"Set manual fullscreen position to (%0, %1)",
	/* Flags */			0,
	/* Params */		(2, PARAM_NUMBER, "Manual fullscreen X coordinate", PARAM_NUMBER, "Manual fullscreen Y coordinate")
)
{
  int myparam1 = Param(TYPE_INT);
  int myparam2 = Param(TYPE_INT);

  //guard against multiple event triggers
  if(GlobalData.gamerectx == myparam1 && GlobalData.gamerecty == myparam2)
    return;

  GlobalData.gamerectx = param1;
  GlobalData.gamerecty = param2;

  UpdateGameRectangleSize(rdPtr);
}


//SetManualFullscreenSize
ACTION
(
	/* ID */			  7,
	/* Name */			"Set manual fullscreen size to (%0, %1)",
	/* Flags */			0,
	/* Params */		(2, PARAM_NUMBER, "Manual fullscreen width", PARAM_NUMBER, "Manual fullscreen height")
)
{
  int myparam1 = Param(TYPE_INT);
  int myparam2 = Param(TYPE_INT);

  //guard against multiple event triggers
  if(GlobalData.gamerectwidth == param1 && GlobalData.gamerectheight == param2)
    return;

  GlobalData.gamerectwidth  = myparam1;
  GlobalData.gamerectheight = myparam2;

  UpdateGameRectangleSize(rdPtr);
}


//ResetManualFullscreen
ACTION
(
	/* ID */			  8,
	/* Name */			"Reset manual fullscreen settings to default",
	/* Flags */			0,
	/* Params */		(0)
)
{
  //guard against multiple event triggers
  if(GlobalData.gamerectx == GlobalData.default_gamerectx &&
  GlobalData.gamerecty == GlobalData.default_gamerecty &&
  GlobalData.gamerectwidth  == GlobalData.default_gamerectwidth &&
  GlobalData.gamerectheight == GlobalData.default_gamerectheight)
    return;

  GlobalData.gamerectx = GlobalData.default_gamerectx;
  GlobalData.gamerecty = GlobalData.default_gamerecty;
  GlobalData.gamerectwidth  = GlobalData.default_gamerectwidth ;
  GlobalData.gamerectheight = GlobalData.default_gamerectheight;

  UpdateGameRectangleSize(rdPtr);
}


//ListenForMaximize
ACTION
(
	/* ID */			  9,
	/* Name */			"Listen for screen maximize",
	/* Flags */			0,
	/* Params */		(0)
)
{
	GlobalData.listen_for_maximize = true;
	rdPtr->rRd->Rehandle();
}



//DontListenForMaximize
ACTION
(
	/* ID */			  10,
	/* Name */			"Don't listen for screen maximize",
	/* Flags */			0,
	/* Params */		(0)
)
{
	GlobalData.listen_for_maximize = false;
}

//Refresh
ACTION
(
    /* ID */			  11,
    /* Name */			"Refresh",
    /* Flags */			0,
    /* Params */		(0)
)
{
    //Sleep(100);
    UpdateGameRectangleSize(rdPtr);
    rdPtr->rRd->Rehandle();
    //rdPtr->rRd->Redisplay();
}

//ListenForMonitorSwitch
ACTION
(
    /* ID */			  12,
    /* Name */			"Listen for monitor switches = %0",
    /* Flags */			0,
    /* Params */		(1, PARAM_NUMBER, "Listen for monitor switches (0 = false, 1 = true)")
)
{
    int myparam1 = Param(TYPE_INT);
    if (myparam1>0)
        GlobalData.listen_for_monitor_switch = true;
    else
        GlobalData.listen_for_monitor_switch = false;
}

//SetPlayArea
ACTION
(
    /* ID */			  13,
    /* Name */			"Set Play Area ( %0x%1 )",
    /* Flags */			0,
    /* Params */		(2, PARAM_NUMBER, "Play Area width", PARAM_NUMBER, "Play Area height")
)
{
    int myparam1 = Param(TYPE_INT);
    int myparam2 = Param(TYPE_INT);

    //guard against multiple event triggers
    if (GlobalData.PlayX == param1 && GlobalData.PlayY == param2)
        return;

    GlobalData.PlayX = myparam1;
    GlobalData.PlayY = myparam2;
 
    UpdateGameRectangleSize(rdPtr);
}

// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================


//GameRectX
EXPRESSION
(
	/* ID */			  0,
	/* Name */			"GameRectX(",
	/* Flags */			0,
	/* Params */		(0)
)
{
  RECT rect = CalculateGameRectangle(rdPtr);
	return rect.left;
}

//GameRectY
EXPRESSION
(
	/* ID */			  1,
	/* Name */			"GameRectY(",
	/* Flags */			0,
	/* Params */		(0)
)
{
  RECT rect = CalculateGameRectangle(rdPtr);
	return rect.top;
}


//GameRectWidth
EXPRESSION
(
	/* ID */			  2,
	/* Name */			"GameRectWidth(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	RECT rect = CalculateGameRectangle(rdPtr);
	return rect.right - rect.left;
}

//GameRectHeight
EXPRESSION
(
	/* ID */			  3,
	/* Name */			"GameRectHeight(",
	/* Flags */			0,
	/* Params */		(0)
)
{
  RECT rect = CalculateGameRectangle(rdPtr);
	return rect.bottom - rect.top;
}




//ManualGameRectX
EXPRESSION
(
	/* ID */			  4,
	/* Name */			"ManualGameRectX(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return GlobalData.gamerectx;
}

//ManualGameRectY
EXPRESSION
(
	/* ID */			  5,
	/* Name */			"ManualGameRectY(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return GlobalData.gamerecty;
}


//ManualGameRectWidth
EXPRESSION
(
	/* ID */			  6,
	/* Name */			"ManualGameRectWidth(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return GlobalData.gamerectwidth;
}

//ManualGameRectHeight
EXPRESSION
(
	/* ID */			  7,
	/* Name */			"ManualGameRectHeight(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return GlobalData.gamerectheight;
}



//CroppedGameWidth
EXPRESSION
(
	/* ID */			  8,
	/* Name */			"CroppedGameWidth(",
	/* Flags */			0,
	/* Params */		(0)
)
{
    RECT rect = CalculateGameRectangle(rdPtr);

    if ((rect.right - rect.left) <= GetScreenWidth(rdPtr->rHo.hoAdRunHeader))
        return GetGameWidth(rdPtr->rHo.hoAdRunHeader);
    int CroppedWidth = (GetGameWidth(rdPtr->rHo.hoAdRunHeader) * GetScreenWidth(rdPtr->rHo.hoAdRunHeader)) / float(rect.right - rect.left);
    if (CroppedWidth % 2 == 0)
        return CroppedWidth;
    else
        return CroppedWidth + 1;

}

//CroppedGameHeight
EXPRESSION
(
	/* ID */			  9,
	/* Name */			"CroppedGameHeight(",
	/* Flags */			0,
	/* Params */		(0)
)
{
    RECT rect = CalculateGameRectangle(rdPtr);

    if ((rect.bottom - rect.top) <= GetScreenHeight(rdPtr->rHo.hoAdRunHeader))
        return GetGameHeight(rdPtr->rHo.hoAdRunHeader);

    int CroppedHeight = (GetGameHeight(rdPtr->rHo.hoAdRunHeader) * GetScreenHeight(rdPtr->rHo.hoAdRunHeader)) / (rect.bottom - rect.top);
    if (CroppedHeight % 2 == 0)
        return CroppedHeight;
    else
        return CroppedHeight + 1;
}




//FullscreenType
EXPRESSION
(
	/* ID */			  10,
	/* Name */			"FullscreenType(",
	/* Flags */			0,
	/* Params */		(0)
)
{
	return (int)GlobalData.fstype;
}

//GameScale
EXPRESSION
(
	/* ID */			  11,
	/* Name */			"GameScale(",
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(0)
)
{
	ReturnFloat(GlobalData.gamescale);
}

//Get Visible Screen Edges
EXPRESSION
(
    /* ID */			  12,
    /* Name */			"LeftEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int leftEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;

    if (GlobalData.fstype == 6) {
        
        return leftEdge - GlobalData.OffsetX / GlobalData.Highest;
    }
    else {
        LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
        float ScreenAspect = GetScreenWidth(rhPtr) / GetScreenHeight(rhPtr);
        float GameAspect = GetGameWidth(rhPtr) / GetGameHeight(rhPtr);
        float ScalingFactor = GetScreenHeight(rhPtr) / GetGameHeight(rhPtr);
        return (GetScreenHeight(rhPtr) * GameAspect - GetScreenHeight(rhPtr) * ScreenAspect) / ScalingFactor / 2;
        //return 0;
    }
}

EXPRESSION
(
    /* ID */			  13,
    /* Name */			"RightEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int leftEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;

    if (GlobalData.fstype == 6) {
        int AppWidth = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCxWin;
        return leftEdge + AppWidth + GlobalData.OffsetX / GlobalData.Highest;
    }
    else
        return 0;
}

EXPRESSION
(
    /* ID */			  14,
    /* Name */			"TopEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int topEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
    if (GlobalData.fstype == 6) {
        return topEdge - GlobalData.OffsetY / GlobalData.Highest;
    }
    else
        return 0;
}

EXPRESSION
(
    /* ID */			  15,
    /* Name */			"BottomEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int topEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
    if (GlobalData.fstype == 6) {
        int AppHeight = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCyWin;
        return topEdge + AppHeight + GlobalData.OffsetY / GlobalData.Highest;
    }
    else
        return 0;
}

//Get Play Area Edges
EXPRESSION
(
    /* ID */			  16,
    /* Name */			"PLeftEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int leftEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
    int AppWidth = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCxWin;
    int BorderWidth = (AppWidth - GlobalData.PlayX) / 2;
    return leftEdge + BorderWidth;
}

EXPRESSION
(
    /* ID */			  17,
    /* Name */			"PRightEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int leftEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayX;
    int AppWidth = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCxWin;
    int BorderWidth = (AppWidth - GlobalData.PlayX) / 2;
    return leftEdge + AppWidth - BorderWidth;
}

EXPRESSION
(
    /* ID */			  18,
    /* Name */			"PTopEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int topEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
    int AppHeight = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCyWin;
    int BorderHeight = (AppHeight - GlobalData.PlayY) / 2;
    return topEdge + BorderHeight;
}

EXPRESSION
(
    /* ID */			  19,
    /* Name */			"PBottomEdge(",
    /* Flags */			0,
    /* Params */		(0)
)
{
    int topEdge = rdPtr->rHo.hoAdRunHeader->rh3.rh3DisplayY;
    int AppHeight = rdPtr->rHo.hoAdRunHeader->rhApp->m_hdr.gaCyWin;
    int BorderHeight = (AppHeight - GlobalData.PlayY) / 2;
    return topEdge + AppHeight - BorderHeight;
}
