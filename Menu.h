// --------------------------------
// Condition menu
// --------------------------------

#ifdef CONDITION_MENU

	ITEM(0, "Is fullscreen")
	ITEM(1, "Is windowed")
  SEPARATOR
  ITEM(2, "On fullscreen")
	ITEM(3, "On windowed")
  ITEM(4, "On change window mode")
	SEPARATOR
	ITEM(5, "On monitor switch")

#endif

// --------------------------------
// Action menu
// --------------------------------

#ifdef ACTION_MENU

	// You have access to edPtr and you can throw in normal C++
	// code wherever you want in any of these menus. If you want to
	// show certain conditions based on a property (a la OINC socket)
	// use the property's value stored in edPtr:

	// if(edPtr->ShowAdvancedOptions)
	// {
	//		SUB_START("Bake pie")
	//			ITEM(0, "Apple pie")
	//			ITEM(1, "Mince pie")
	//		SUB_END
	// }
	// else
	// {
	//		ITEM(0, "Bake apple pie")
	// }

	// You could do some pretty cool stuff with this, like loading a
	// menu from an external source (ActiveX object?)

	// for(int i=0;i<edPtr->ActionCount;i++)
	//		ITEM(i,edPtr->Actions[i])

	// This will run every time the menu is opened in MMF2, so don't
	// make it too slow or it could get annoying

  ITEM(0,"Go fullscreen")
  ITEM(1,"Go windowed")
  SEPARATOR
	SUB_START("Automatic adjustments")
    ITEM(4,"Set fullscreen type")
	  ITEM(5,"Set game scale")
  SUB_END
  SUB_START("Manual adjustments")
	ITEM(13, "Set Play Area size")
	SEPARATOR
    ITEM(6,"Set manual fullscreen position")
    ITEM(7,"Set manual fullscreen size")
    ITEM(8,"Reset manual fullscreen settings to default")
  SUB_END
  SEPARATOR
  SUB_START("Misc.")
	ITEM(11, "Refresh")
	ITEM(12, "Listen for monitor switch? (Default = TRUE)")
	SEPARATOR
    ITEM(2,"Apply fullscreen settings to windowed size")
	ITEM(3,"Don't apply fullscreen settings to windowed size (default)")
	SEPARATOR
	ITEM(9,"Listen for screen maximize (default)")
	ITEM(10,"Don't listen for screen maximize") 
  SUB_END
  SEPARATOR

#endif


// --------------------------------
// Expression menu
// --------------------------------

#ifdef EXPRESSION_MENU


  SUB_START("Game rectangle")
    ITEM(0,"Game rectangle X (after scaling)")
    ITEM(1,"Game rectangle Y (after scaling)")
    ITEM(2,"Game rectangle width  (after scaling)")
    ITEM(3,"Game rectangle height (after scaling)")
  SUB_END
  SUB_START("Manual settings")
    ITEM(4,"Manual Game rectangle X")
    ITEM(5,"Manual Game rectangle Y")
    ITEM(6,"Manual Game rectangle width ")
    ITEM(7,"Manual Game rectangle height")
  SUB_END
  SUB_START("Cropping")
	  ITEM(8,"Cropped game width  (before scaling)")
	  ITEM(9,"Cropped game height (before scaling)")
  SUB_END
  SUB_START("Visible Screen Edges")
		ITEM(12, "Get Left Edge")
		ITEM(13, "Get Right Edge")
		ITEM(14, "Get Top Edge")
		ITEM(15, "Get Bottom Edge")
  SUB_END
	  SUB_START("Play Area Edges")
	  ITEM(16, "Get Left Edge")
	  ITEM(17, "Get Right Edge")
	  ITEM(18, "Get Top Edge")
	  ITEM(19, "Get Bottom Edge")
	  SUB_END
  ITEM(10,"Fullscreen type")
  ITEM(11,"Game scale")

#endif