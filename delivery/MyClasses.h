#pragma once

#include "Source.h"

// —труктура, котора€ описывает точки на карте
value struct MyPoint
{
public:
	int x, y;
	char type; // 0 - склад, 1 - магазин, 2 - дом, 3 - перекресток
	System::Collections::Generic::List<MyPoint>^ points;
};

//  ласс, которые описывает транспортные средства
ref class Transport
{
public:
	void move();
private:
	void print_picture();
	void choose_new_point();
	void start_event();
	int speed;
	int x, y;
	int index; // «анул€ть, при создании нового points_path
	int step;
	bool isMoving;
	Direction direction;
	MyPoint& departure_point;
	MyPoint& destination_point;
	array<MyPoint>^ points_path;
	System::Windows::Forms::PictureBox^ picBox;
	System::Windows::Forms::Control^ par;
	String^ name;
};

//  ласс, который описывает здани€
class Structure
{
public:
	const MyPoint& point;
};

//  ласс, который описывает работу склада
class Warehouse : Structure
{
public:
	void load();
};

//  ласс, который описывает работу магазина
class Store : Structure
{
public:
	void load();
	void unload();
};

//  ласс, который описывает работу дома-получател€
class House : Structure
{
public:
	void unload();
};