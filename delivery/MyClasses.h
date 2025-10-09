#pragma once

#include "Source.h"

using namespace System;
using namespace System::Windows::Forms;

ref class Structure;

// —труктура, котора€ описывает точки на карте
ref class MyPoint
{
public:
	int number;
	int x, y;
	array<MyPoint^>^ to_points;
	Structure^ structure;
	MyPoint(int num, int x, int y, char type);
};

//  ласс, которые описывает транспортные средства
ref class Transport abstract
{
public:
	delegate void LoadEventHandler(Transport^ sender, Structure^ target);
	event LoadEventHandler^ LoadEvent;

	delegate void UnloadEventHandler(Transport^ sender, Structure^ target);
	event UnloadEventHandler^ UnloadEvent;

	void move();
	virtual void start_event() = 0;
private:
	int index; // «анул€ть, при создании нового points_path
	bool isMoving;
	Direction direction;
	array<MyPoint^>^ points_path;
	void print_picture();
	void choose_new_point();
protected:
	int x, y;
	int step;
	String^ name;
	MyPoint^ departure_point;
	MyPoint^ destination_point;
	System::Windows::Forms::Control^ par;
	System::Windows::Forms::PictureBox^ pic_box;
};

//  ласс, который описывает велосипедистов, доставл€ющих товары
ref class Bicycle : Transport
{
public:
	Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	void start_event() override;
};

//  ласс, который описывает машины, доставл€ющие товары
ref class Car : Transport
{
public:
	Car(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	void start_event() override;
};

//  ласс, который описывает здани€
ref class Structure abstract
{
public:
	const MyPoint^ point;
	Structure(MyPoint^ point);
	Structure();
	virtual void subscribe_if_relevant(Transport^ t) = 0;
};

//  ласс, который описывает работу склада
ref class Warehouse : Structure
{
public:
	Warehouse(int number_point_car);
	void subscribe_if_relevant(Transport^ t) override;
private:
	void load(Transport^ sender, Structure^ target);
};

//  ласс, который описывает работу магазина
ref class Store : Structure
{
public:
	MyPoint^ bicycle_point;
	MyPoint^ car_point;
	Store(int num_point_bicycle, int num_point_car);
	void subscribe_if_relevant(Transport^ t) override;
private:
	void load(Transport^ sender, Structure^ target);
	void unload(Transport^ sender, Structure^ target);
};

//  ласс, который описывает работу дома-получател€
ref class House : Structure
{
public:
	House(int num_point_bicycle);
	void subscribe_if_relevant(Transport^ t) override;
private:
	void unload(Transport^ sender, Structure^ target);
};