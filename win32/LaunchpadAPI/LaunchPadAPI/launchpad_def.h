/**
* This is an implementation for Novation Launchpad Mini-
* @TODO: abstract this to be working for multiple launchpads
*/

#ifndef __LAUNCHPAD_DEF_H__
#define __LAUNCHPAD_DEF_H__

typedef unsigned char LP_KEY;
typedef unsigned char LP_LINE;
typedef unsigned char LP_COLUMN;
typedef unsigned char LP_CONTROLLER;

enum LP_COLOR
{
	COL_NONE = 0,
	COL_DARKER_RED = 1,
	COL_DARK_RED = 2,
	COL_RED = 3,
	COL_DARKER_GREEN = 16,
	COL_DARK_GREEN = 32,
	COL_GREEN = 48
};

#define COL_DARKER_AMBRE = COL_DARKER_GREEN + COL_DARKER_RED
#define COL_AMBRE = COL_DARKER_GREEN + COL_DARKER_RED

#define FLAG_DISPLAY_NORMAL			12
#define FLAG_DISPLAY_FLASH			8
#define FLAG_DISPLAY_DBUFFER		0

#define CTRL_TOP_SIZE				8
#define CTRL_RIGHT_SIZE				8

#define LINE_LENGTH					8
#define LINE_INCR_OFFSET			8
#define LINE_LENGTH_WCTRL			9
#define COL_LENGTH					8

// transformers (W to raw)
#define CLR_LINE_CTRL_SIDE(k)		((k) + ((k) / LINE_LENGTH_WCTRL))
#define LINE_APPLY_OFFSET(k)		((k) + ((k) / LINE_LENGTH) * LINE_INCR_OFFSET)
#define LINE_MOVE(l, v)				(((l) * LINE_LENGTH) + (v))
#define COL_MOVE(c, v)				((c) + ((v) * (LINE_LENGTH_WCTRL)))

// transformers (raw to W)
#define RAW_TO_LINE(k)				((k) / (LINE_INCR_OFFSET + LINE_LENGTH))
#define RAW_TO_COLUMN(k)			((k) % (LINE_LENGTH + LINE_INCR_OFFSET))
#define RAW_TO_CTRL(k)				(RAW_TO_COLUMN((k)) == 8 ? (CTRL_TOP_SIZE + RAW_TO_LINE(k)) : 0 )

// transformers (W to XY)
#define KEY_TO_LINE(k)				((k) / LINE_LENGTH)
#define KEY_TO_COLUMN(k)			((k) % LINE_LENGTH)

// transformers (XY to W)
#define XYKEY_TO_W(x, y)			((x) + ((y) * LINE_LENGTH))

struct KeyStyle
{
	bool blink;
	LP_COLOR color;

	KeyStyle(bool _blink = false, LP_COLOR _color = COL_RED)
		: blink(_blink), color(_color)
	{
		
	}

	unsigned char concatToChar()
	{
		return (color + FLAG_DISPLAY_NORMAL);
	}
};

#define EVENT_KEY_CHANGED			144
#define EVENT_CTRL_CHANGED			176

#define KEY_PRESSED					127
#define KEY_RELEASED				0

struct LaunchPadEvent
{
	double time;
	unsigned char id;
	unsigned char key;
	unsigned char state;

	LP_LINE	line;
	LP_COLUMN col;
};

#endif // !__LAUNCHPAD_DEF_H__
