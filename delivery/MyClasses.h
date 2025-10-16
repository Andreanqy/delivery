#pragma once

#include "Source.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

ref class Structure;
ref class Store;

// Структура, которая описывает точки на карте
ref class MyPoint
{
public:
	int number;
	int x, y;
	array<MyPoint^>^ to_points;
	Structure^ structure;
	MyPoint(int num, int x, int y, char type);
};

// Класс, который отвечает за отрисовку анимации загрузки/разгрузки
ref class LoaderAnimation
{
public:
	int x1;
	int x2;
	int y;
	int step;
	int steps;
	int current;
	int total_steps;
	int goal_steps;
	PictureBox^ loader;
	Transport^ transport;
	System::String^ direction;

	bool update();

	LoaderAnimation(Structure^ source, Transport^ target, int size, int goal_steps);
};

// Класс, которые описывает транспортные средства
ref class Transport abstract
{
public:
	delegate void LoadEventHandler(Transport^ sender, Structure^ target);
	event LoadEventHandler^ LoadEvent;

	delegate void UnloadEventHandler(Transport^ sender, Structure^ target);
	event UnloadEventHandler^ UnloadEvent;

	int x, y;
	int index;
	bool isMoving;

	Direction direction;

	MyPoint^ departure_point;
	MyPoint^ destination_point;

	array<MyPoint^>^ points_path;

	System::Windows::Forms::PictureBox^ pic_box;

	void move();
	Store^ get_random_store();
	virtual void start_event() = 0;
	void log(System::String^ label);
	void play_loader_animation(Structure^ source, Transport^ target, int size, int goal_steps);

	Transport(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);

private:
	void choose_new_destination_point();
	void print_picture(Direction prev_direction);

protected:
	int step;
	System::String^ name;
	System::Windows::Forms::Control^ par;
};

// Класс, который описывает велосипедистов, доставляющих товары
ref class Bicycle : Transport
{
public:
	int cargo_index;
	int current_load;
	System::String^ name_number;
	array<System::Tuple<MyPoint^, int>^>^ delivery_plan;

	void start_event() override;

	Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
};

// Класс, который описывает машины, доставляющие товары
ref class Car : Transport
{
public:
	void start_event() override;

	Car(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
};

// Класс, который описывает здания
ref class Structure abstract
{
public:
	MyPoint^ point;

	virtual void subscribe_if_relevant(Transport^ t) = 0;

	Structure();
	Structure(MyPoint^ point);
};

// Класс, который описывает работу склада
ref class Warehouse : Structure
{
public:
	void subscribe_if_relevant(Transport^ t) override;

	Warehouse(int number_point_car);

private:
	void load(Transport^ sender, Structure^ target);
};

// Класс, который описывает работу магазина
ref class Store : Structure
{
public:
	MyPoint^ car_point;
	MyPoint^ bicycle_point;

	void subscribe_if_relevant(Transport^ t) override;

	Store(int num_point_bicycle, int num_point_car);

private:
	void load(Transport^ sender, Structure^ target);
	void unload(Transport^ sender, Structure^ target);
	array<System::Tuple<MyPoint^, int>^>^ create_delivery_plan();
};

// Класс, который описывает работу дома-получателя
ref class House : Structure
{
public:
	void subscribe_if_relevant(Transport^ t) override;

	House(int num_point_bicycle);

private:
	void unload(Transport^ sender, Structure^ target);
};