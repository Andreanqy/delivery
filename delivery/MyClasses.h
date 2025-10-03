#pragma once

enum Direction {Right, Left, Up, Down};

// ���������, ������� ��������� ����� �� �����
struct MyPoint {
public:
	int x, y;
	char type; // 0 - �����, 1 - �������, 2 - ���, 3 - �����������
};

// �����, ������� ��������� ������������ ��������
class Transport {
public:
	void move(int x, int y);
private:
	void turn();
	void choose_new_point();
	int speed;
	int x, y;
	Direction direction;
};

// �����, ������� ��������� ������
class Structure {
public:
	const MyPoint& point;
};

// �����, ������� ��������� ������ ������
class Warehouse : Structure {
public:
	void load();
};

// �����, ������� ��������� ������ ��������
class Store : Structure {
public:
	void load();
	void unload();
};

// �����, ������� ��������� ������ ����-����������
class House : Structure {
public:
	void unload();
};