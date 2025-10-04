#pragma once

#include "Source.h"

// ���������, ������� ��������� ����� �� �����
value struct MyPoint
{
public:
	int x, y;
	char type; // 0 - �����, 1 - �������, 2 - ���, 3 - �����������
	System::Collections::Generic::List<MyPoint>^ points;
};

// �����, ������� ��������� ������������ ��������
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
	int index; // ��������, ��� �������� ������ points_path
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

// �����, ������� ��������� ������
class Structure
{
public:
	const MyPoint& point;
};

// �����, ������� ��������� ������ ������
class Warehouse : Structure
{
public:
	void load();
};

// �����, ������� ��������� ������ ��������
class Store : Structure
{
public:
	void load();
	void unload();
};

// �����, ������� ��������� ������ ����-����������
class House : Structure
{
public:
	void unload();
};