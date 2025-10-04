#include "Source.h"

System::String^ string(Direction direction)
{
	switch (direction)
	{
	case Up: return "Up"; break;
	case Down: return "Down"; break;
	case Left: return "Left"; break;
	case Right: return "Right"; break;
	}
}