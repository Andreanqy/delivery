#pragma once

#include "Source.h"

using namespace System;
using namespace System::Windows::Forms;

ref class Structure;

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
ref class Transport
{
public:
	delegate void LoadEventHandler(Transport^ sender, Structure^ target);
	event LoadEventHandler^ LoadEvent;
	delegate void UnloadEventHandler(Transport^ sender, Structure^ target);
	event UnloadEventHandler^ UnloadEvent;
	void move();
private:
	int index; // ��������, ��� �������� ������ points_path
	bool isMoving;
	Direction direction;
	array<MyPoint^>^ points_path;
	void print_picture();
	void choose_new_point();
	virtual void start_event();
protected:
	int x, y;
	int step;
	String^ name;
	MyPoint^ departure_point;
	MyPoint^ destination_point;
	System::Windows::Forms::Control^ par;
	System::Windows::Forms::PictureBox^ pic_box;
};

// �����, ������� ��������� ��������������, ������������ ������
ref class Bicycle : Transport
{
public:
	Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point);
private:
	void start_event() override;
};

// �����, ������� ��������� ������, ������������ ������
ref class Car : Transport
{
public:
	Car(int x, int y, MyPoint^ departure_point, MyPoint^ destination_point);
private:
	void start_event() override;
};

// �����, ������� ��������� ������
ref class Structure
{
public:
	const MyPoint^ point;
	Structure(MyPoint^ point);
	Structure();
	virtual void subscribe_if_relevant(Transport^ t);
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