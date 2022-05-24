#ifndef DATA_H
#define DATA_H

class rRundata;
typedef rRundata * LPRRDATA;

enum FULLSCREENTYPE{BESTFIT=0, BESTPIXELPERFECTFIT=1, ONETOONE=2, MATCHWIDTH=3, MATCHHEIGHT=4, MANUAL=5, RESIZEWITHBORDER=6};





// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

typedef struct tagEDATA_V1
{
	extHeader		eHeader;
	short			swidth;
	short			sheight;

} EDITDATA;
typedef EDITDATA * LPEDATA;



// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here


typedef struct tagRDATA
{
	#include "MagicRDATA.h"


//store all this crap globally instead
/*
  bool fullscreen_initialized;    //we don't initialize it right away, so we need to remember if we haven't done it yet
  bool fullscreen;  //remember what mode we're in
  bool apply_fs_settings_to_window;   //whether or not we should make the window sized the same as our fullscreen game rectangle would be
  bool listen_for_maximize;       //whether we should update the size automatically when the screen gets maximized


  //NOTE: these values only apply to manual mode
  int gamerectx;    //x position of the game rectangle when in fullscreen
  int gamerecty;    //y position of game rectangle
  int gamerectwidth;  //width of the game rectangle when in fullscreen
  int gamerectheight; //height of the game rectangle
  int default_gamerectx;  //obtained at the start and then never changed
  int default_gamerecty;
  int default_gamerectwidth;
  int default_gamerectheight;


	FULLSCREENTYPE fstype;  //governs how we decide to size the game rectangle
  float gamescale;        //1.0 by default, this is multiplied to the size obtained from the fstype calculation
*/



} RUNDATA;
typedef	RUNDATA	* LPRDATA;




typedef struct _GLOBALDATA
{
  bool fullscreen_initialized;    //we don't initialize it right away, so we need to remember if we haven't done it yet
  bool fullscreen;  //remember what mode we're in
  bool apply_fs_settings_to_window;   //whether or not we should make the window sized the same as our fullscreen game rectangle would be
  bool listen_for_maximize;       //whether we should update the size automatically when the screen gets maximized
  bool listen_for_monitor_switch;       //Check if window switched monitors


  //NOTE: these values only apply to manual mode
  int gamerectx;    //x position of the game rectangle when in fullscreen
  int gamerecty;    //y position of game rectangle
  int gamerectwidth;  //width of the game rectangle when in fullscreen
  int gamerectheight; //height of the game rectangle
  int default_gamerectx;  //obtained at the start and then never changed
  int default_gamerecty;
  int default_gamerectwidth;
  int default_gamerectheight;
  HMONITOR previous_monitor;
  int AppWidth;
  int AppHeight;
  int PlayX;
  int PlayY;
  int Highest;
  int OffsetX;
  int OffsetY;


  FULLSCREENTYPE fstype;  //governs how we decide to size the game rectangle
  float gamescale;        //1.0 by default, this is multiplied to the size obtained from the fstype calculation
} GLOBALDATA;



extern GLOBALDATA GlobalData;






#endif