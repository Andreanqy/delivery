#include "MyClasses.h"
#include "MyForm.h"

MyPoint::MyPoint(int num, int x, int y, char type) {
	number = num;
	this->x = x;
	this->y = y;
}

void Transport::move()
{
	if (isMoving)
	{
		switch (direction)
		{
		case Up: y -= step; break;
		case Down: y += step; break;
		case Left: x -= step; break;
		case Right: x += step; break;
		}
		if (direction == Up && destination_point->y >= y ||
			direction == Down && destination_point->y <= y ||
			direction == Left && destination_point->x >= x ||
			direction == Right && destination_point->x <= x)
		{
			x = destination_point->x;
			y = destination_point->y;
			isMoving = points_path->Length != ++index;
			if (isMoving) choose_new_destination_point();
			else start_event();
		}
		print_picture();
	}
}

array<MyPoint^>^ Transport::create_path(MyPoint^ departure_point, MyPoint^ global_destination_point)
{
	System::Collections::Generic::Queue<MyPoint^>^ queue = gcnew System::Collections::Generic::Queue<MyPoint^>();
	System::Collections::Generic::Dictionary<int, MyPoint^>^ cameFrom = gcnew System::Collections::Generic::Dictionary<int, MyPoint^>();
	System::Collections::Generic::HashSet<int>^ visited = gcnew System::Collections::Generic::HashSet<int>();

	queue->Enqueue(departure_point);
	visited->Add(departure_point->number);
	cameFrom->Add(departure_point->number, nullptr);

	while (queue->Count > 0)
	{
		MyPoint^ current = queue->Dequeue();

		if (current->number == global_destination_point->number)
		{
			System::Collections::Generic::List<MyPoint^>^ path = gcnew System::Collections::Generic::List<MyPoint^>();
			MyPoint^ step = current;
			while (step != nullptr)
			{
				path->Add(step);
				step = cameFrom[step->number];
			}
			path->Reverse();
			return path->ToArray();
		}

		for each (MyPoint ^ neighbor in current->to_points)
		{
			if (!visited->Contains(neighbor->number))
			{
				visited->Add(neighbor->number);
				cameFrom->Add(neighbor->number, current);
				queue->Enqueue(neighbor);
			}
		}
	}

	return gcnew array<MyPoint^>(0);
}

void Transport::choose_new_destination_point()
{
	departure_point = destination_point;
	destination_point = points_path[index];
	int dif_x = destination_point->x - departure_point->x;
	int dif_y = destination_point->y - departure_point->y;
	if (dif_x > 0) direction = Right;
	else if (dif_x < 0) direction = Left;
	else if (dif_y > 0) direction = Down;
	else if (dif_y < 0) direction = Up;
}

void Transport::print_picture()
{
	pic_box->Image = Image::FromFile(delivery::MyForm::path_to_resource + name + "_" + string(direction) + ".png");
	pic_box->Location = System::Drawing::Point(x - pic_box->Size.Width/2, y - pic_box->Size.Height/2);
}

Transport::Transport(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, Control^ parent)
{
	this->x = x;
	this->y = y;
	this->isMoving = true;
	this->points_path = create_path(departure_point, global_destination_point);
	this->departure_point = departure_point;
	this->destination_point = this->points_path[1];
	this->pic_box = gcnew System::Windows::Forms::PictureBox();
	this->pic_box->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	this->pic_box->BackColor = System::Drawing::Color::Transparent;
	this->pic_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->pic_box->Show();
	parent->Controls->Add(pic_box);
}

Bicycle::Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, Control^ parent) : Transport(x, y, departure_point, global_destination_point, parent)
{
	this->pic_box->Image = Image::FromFile(delivery::MyForm::path_to_resource + "bicycle_Up.png");
	this->pic_box->Location = System::Drawing::Point(x - 10, y - 20);
	this->pic_box->MaximumSize = System::Drawing::Size(20, 40);
	this->pic_box->Size = System::Drawing::Size(20, 40);
	this->name = "bicycle";
	this->step = 10;
}

Car::Car(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, Control^ parent) : Transport(x, y, departure_point, global_destination_point, parent)
{
	this->pic_box->Image = Image::FromFile(delivery::MyForm::path_to_resource + "car_Up.png");
	this->pic_box->Location = System::Drawing::Point(x - 20, y - 40);
	this->pic_box->MaximumSize = System::Drawing::Size(40, 80);
	this->pic_box->Size = System::Drawing::Size(40, 80);
	this->name = "car";
	this->step = 20;
}

void Bicycle::start_event()
{
	if (Store^ st = dynamic_cast<Store^>(destination_point->structure)) LoadEvent(this, destination_point->structure);
	else if (House^ h = dynamic_cast<House^>(destination_point->structure)) UnloadEvent(this, destination_point->structure);
}

void Car::start_event()
{
	if (Warehouse^ wh = dynamic_cast<Warehouse^>(destination_point->structure)) LoadEvent(this, destination_point->structure);
	else if (Store^ st = dynamic_cast<Store^>(destination_point)) UnloadEvent(this, destination_point->structure);
}

Structure::Structure(MyPoint^ point)
{
	this->point = point;
}

Structure::Structure() {}

Warehouse::Warehouse(int num_point_car) : Structure(delivery::MyForm::points_car[num_point_car])
{
	delivery::MyForm::points_car[num_point_car]->structure = this;
}

void Warehouse::subscribe_if_relevant(Transport^ t)
{
	if (Car^ car = dynamic_cast<Car^>(t))
		car->LoadEvent += gcnew Transport::LoadEventHandler(this, &Warehouse::load);
}

void Warehouse::load(Transport^ sender, Structure^ target)
{
	if (target == this) { /*Загрузка машины*/ }
}

Store::Store(int num_point_bicycle, int num_point_car)
{
	this->bicycle_point = delivery::MyForm::points_bicycle[num_point_bicycle];
	this->car_point = delivery::MyForm::points_car[num_point_car];
	delivery::MyForm::points_bicycle[num_point_bicycle]->structure = this;
	delivery::MyForm::points_car[num_point_car]->structure = this;
}

void Store::subscribe_if_relevant(Transport^ t)
{
	if (Bicycle^ bicycle = dynamic_cast<Bicycle^>(t))
		bicycle->LoadEvent += gcnew Transport::LoadEventHandler(this, &Store::load);
	if (Car^ car = dynamic_cast<Car^>(t))
		car->UnloadEvent += gcnew Transport::UnloadEventHandler(this, &Store::unload);
}

void Store::load(Transport^ sender, Structure^ target)
{
	if (target == this) { /*Загрузка велосипедиста*/ }
}

void Store::unload(Transport^ sender, Structure^ target)
{
	if (target == this) { /*Разгрузка машины*/ }
}

House::House(int num_point_bicycle) : Structure(delivery::MyForm::points_bicycle[num_point_bicycle])
{
	delivery::MyForm::points_bicycle[num_point_bicycle]->structure = this;
}

void House::subscribe_if_relevant(Transport^ t)
{
	if (Bicycle^ bicycle = dynamic_cast<Bicycle^>(t))
		bicycle->UnloadEvent += gcnew Transport::UnloadEventHandler(this, &House::unload);
}

void House::unload(Transport^ sender, Structure^ target)
{
	if (target == this) { /*Разгрузка велосипедиста*/ }
}