#include "MyClasses.h"

void Transport::move()
{
	switch (direction)
	{
	case Up: y -= step; break;
	case Down: y += step; break;
	case Left: x -= step; break;
	case Right: x += step; break;
	default:
		if (direction == Up && destination_point.y >= y ||
			direction == Down && destination_point.y <= y ||
			direction == Left && destination_point.x >= x ||
			direction == Right && destination_point.x <= x)
		{
			x = destination_point.x;
			y = destination_point.y;
			isMoving = points_path->Length != ++index;
			if (isMoving) choose_new_point();
			else start_event();
		}
		print_picture();
	}
}

void Transport::choose_new_point()
{
	departure_point = destination_point;
	destination_point = points_path[index];
	int dif_x = destination_point.x - departure_point.x;
	int dif_y = destination_point.y - departure_point.y;
	if (dif_x > 0) direction = Right;
	else if (dif_x < 0) direction = Left;
	else if (dif_y > 0) direction = Down;
	else if (dif_y < 0) direction = Up;
}

void Transport::print_picture()
{
	picBox->Image = Image::FromFile(name + string(direction) + ".png");
	picBox->Location = System::Drawing::Point(x - picBox->Image->Size.Width/2, y - picBox->Image->Size.Height/2);
}

void Transport::start_event()
{
	if (name == "vehicle" /*и подъехал к магазину*/)
	{
		//...
	}
	else if (name == "vehicle" /*и подъехал к дому*/)
	{
		//...
	}
	else if (name == "car" /*и подъехал ко складу*/)
	{
		//...
	}
	else if (name == "car" /*и подъехал к магазину*/)
	{
		//...
	}
}