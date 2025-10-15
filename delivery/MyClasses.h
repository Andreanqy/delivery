#pragma once

#include "Source.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

ref class Structure;
ref class Store;

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

	Transport(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	int x, y;
	int index; // «анул€ть, при создании нового points_path
	bool isMoving;
	MyPoint^ departure_point;
	MyPoint^ destination_point;
	array<MyPoint^>^ points_path;
	System::Windows::Forms::PictureBox^ pic_box;
	void move();
	virtual void start_event() = 0;
	array<MyPoint^>^ create_path(MyPoint^ departure_point, MyPoint^ global_destination_point);
	Store^ get_random_store();
	void play_loader_animation(Structure^ source, Transport^ target, int size);
private:
	Direction direction;
	void print_picture();
	void choose_new_destination_point();
protected:
	int step;
	String^ name;
	System::Windows::Forms::Control^ par;
};

//  ласс, который описывает велосипедистов, доставл€ющих товары
ref class Bicycle : Transport
{
public:
	int current_load;
	array<Tuple<MyPoint^, int>^>^ delivery_plan;
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
	array<Tuple<MyPoint^, int>^>^ create_delivery_plan();
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