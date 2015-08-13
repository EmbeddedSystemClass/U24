#ifndef _TOUCH_DEFINE_H_
#define _TOUCH_DEFINE_H_

#include "../UnifyInterface/FacTestToolInterface.h"

#define TouchBlock_MAX_H_Number 96
#define TouchBlock_MAX_V_Number 160

#define TouchPoint_MAX_Number 4

//UI display block
typedef struct tagTouchBlockForm
{
	int x_Left;
	int y_Top;
	int x_Right;
	int y_Bottom;

	bool Status;//true:Touched;false:not touched

}TouchBlockForm;

//Touch Point
typedef struct tagTouchPointForm
{
	int x;
	int y;
}TouchPointForm;

#endif // End of #ifndef _COMMON_DEFINE_H_
