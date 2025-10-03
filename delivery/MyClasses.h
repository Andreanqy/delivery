#pragma once

enum Direction {Right, Left, Up, Down};

// —труктура, котора€ описывает точки на карте
struct MyPoint {
public:
	int x, y;
	char type; // 0 - склад, 1 - магазин, 2 - дом, 3 - перекресток
};

//  ласс, которые описывает транспортные средства
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

//  ласс, который описывает здани€
class Structure {
public:
	const MyPoint& point;
};

//  ласс, который описывает работу склада
class Warehouse : Structure {
public:
	void load();
};

//  ласс, который описывает работу магазина
class Store : Structure {
public:
	void load();
	void unload();
};

//  ласс, который описывает работу дома-получател€
class House : Structure {
public:
	void unload();
};