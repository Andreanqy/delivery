#pragma once

#include "Source.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

ref class Structure;
ref class Store;

// ���������, ������� ��������� ����� �� �����
ref class MyPoint
{
public:
	int number;
	int x, y;
	array<MyPoint^>^ to_points;
	Structure^ structure;
	MyPoint(int num, int x, int y, char type);
};

// �����, ������� ��������� ������������ ��������
ref class Transport abstract
{
public:
	delegate void LoadEventHandler(Transport^ sender, Structure^ target);
	event LoadEventHandler^ LoadEvent;

	delegate void UnloadEventHandler(Transport^ sender, Structure^ target);
	event UnloadEventHandler^ UnloadEvent;

	Transport(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	int x, y;
	int index; // ��������, ��� �������� ������ points_path
	bool isMoving;
	MyPoint^ departure_point;
	MyPoint^ destination_point;
	array<MyPoint^>^ points_path;
	System::Windows::Forms::PictureBox^ pic_box;
	void move();
	virtual void start_event() = 0;
	Direction direction;
	//array<MyPoint^>^ create_path(MyPoint^ departure_point, MyPoint^ global_destination_point);
	Store^ get_random_store();
	void play_loader_animation(Structure^ source, Transport^ target, int size, int goal_steps);
	void log(System::String^ label);
private:
	void print_picture(Direction prev_direction);
	void choose_new_destination_point();
protected:
	int step;
	System::String^ name;
	System::Windows::Forms::Control^ par;
};

// �����, ������� ��������� ��������������, ������������ ������
ref class Bicycle : Transport
{
public:
	int current_load;
	array<System::Tuple<MyPoint^, int>^>^ delivery_plan;
	int cargo_index;
	System::String^ name_number;
	Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	void start_event() override;
};

// �����, ������� ��������� ������, ������������ ������
ref class Car : Transport
{
public:
	Car(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point, Control^ parent);
	void start_event() override;
};

// �����, ������� ��������� ������
ref class Structure abstract
{
public:
	MyPoint^ point;
	Structure(MyPoint^ point);
	Structure();
	virtual void subscribe_if_relevant(Transport^ t) = 0;
};

// �����, ������� ��������� ������ ������
ref class Warehouse : Structure
{
public:
	Warehouse(int number_point_car);
	void subscribe_if_relevant(Transport^ t) override;
private:
	void load(Transport^ sender, Structure^ target);
};

// �����, ������� ��������� ������ ��������
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
	array<System::Tuple<MyPoint^, int>^>^ create_delivery_plan();
};

// �����, ������� ��������� ������ ����-����������
ref class House : Structure
{
public:
	House(int num_point_bicycle);
	void subscribe_if_relevant(Transport^ t) override;
private:
	void unload(Transport^ sender, Structure^ target);
};