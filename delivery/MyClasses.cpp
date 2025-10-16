#include "MyClasses.h"
#include "MyForm.h"
#include "Source.h"

MyPoint::MyPoint(int num, int x, int y, char type) {
	number = num;
	this->x = x;
	this->y = y;
}

LoaderAnimation::LoaderAnimation(Structure^ source, Transport^ target, int size, int goal_steps)
{
	if (Bicycle^ b = dynamic_cast<Bicycle^>(target))
	{
		if (Store^ s = dynamic_cast<Store^>(source))
		{
			switch (s->bicycle_point->number)
			{
			case 0: x1 = s->bicycle_point->x + 40; x2 = target->x; break;
			case 1: x1 = s->bicycle_point->x - 40; x2 = target->x - 15; break;
			}
		}
		else if (House^ h = dynamic_cast<House^>(source))
		{
			switch (h->point->number)
			{
			case 2:
			case 11:
			case 13: x1 = h->point->x - 40; x2 = target->x - 15; break;
			case 12:
			case 16: x1 = h->point->x + 40; x2 = target->x - 15; break;
			}
		}
	}
	else if (Car^ c = dynamic_cast<Car^>(target))
	{
		if (Store^ s = dynamic_cast<Store^>(source))
		{
			switch (s->car_point->number)
			{
			case 1: x1 = s->car_point->x - 60; x2 = target->x - 15; break;
			case 2: x1 = s->car_point->x + 60; x2 = target->x - 15; break;
			}
		}
		if (Warehouse^ wh = dynamic_cast<Warehouse^>(source))
		{
			x1 = wh->point->x - 90;
			x2 = target->x - 30;
		}
	}

	y = target->y;
	step = 5;
	steps = abs(x2 - x1) / step;
	current = 0;
	total_steps = 0;
	this->goal_steps = goal_steps;
	transport = target;
	direction = (x2 - x1 > 0) ? "Right" : "Left";

	System::Windows::Forms::Control^ parent = target->pic_box->Parent;
	loader = gcnew System::Windows::Forms::PictureBox();
	loader->Image = System::Drawing::Image::FromFile(gcnew System::String(delivery::path_to_resource) + "loader_" + direction + ".png");
	loader->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	loader->BackColor = System::Drawing::Color::Transparent;
	loader->Size = System::Drawing::Size(size, size);
	loader->Location = System::Drawing::Point(x1, y);
	parent->Controls->Add(loader);
	loader->BringToFront();
}

bool LoaderAnimation::update()
{
	loader->Location = System::Drawing::Point(loader->Location.X + (direction == "Right" ? step : -step), y - (loader->Size.Height / 2));
	current++;
	total_steps++;
	if (current >= steps)
	{
		direction = (direction == "Right") ? "Left" : "Right";
		loader->Image = System::Drawing::Image::FromFile(gcnew System::String(delivery::path_to_resource) + "loader_" + direction + ".png");
		current = 0;
	}

	if (total_steps == goal_steps)
	{
		loader->Parent->Controls->Remove(loader);
		delete loader;
		loader = nullptr;
		transport->isMoving = true;
		return true;
	}

	return false;
}

void Transport::move()
{
	if (!isMoving || destination_point == nullptr) return;

	Direction prev_direction = direction;

	log("Before move");

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
		log("Reached point");
		index++;
		if (index < points_path->Length)
		{
			isMoving = true;
			log("Next point set");
		}
		else
		{
			isMoving = false;
			log("End of path");
			index = 0;
			start_event();
		}
		choose_new_destination_point();
	}
	print_picture(prev_direction);
}

Store^ Transport::get_random_store()
{
	System::Collections::Generic::List<Store^>^ stores = gcnew System::Collections::Generic::List<Store^>();
	for each (Structure ^ s in delivery::MyForm::structures)
	{
		if (Store^ st = dynamic_cast<Store^>(s))
			stores->Add(st);
	}

	System::Random^ rnd = gcnew System::Random();
	return stores[rnd->Next(stores->Count)];
}

void Transport::play_loader_animation(Structure^ source, Transport^ target, int size, int goal_steps)
{
	delivery::MyForm::active_animations->Add(gcnew LoaderAnimation(source, target, size, goal_steps));
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

void Transport::print_picture(Direction prev_direction)
{
	if (prev_direction != direction)
		pic_box->Image = System::Drawing::Image::FromFile(delivery::MyForm::path_to_resource + name + "_" + string(direction) + ".png");

	pic_box->Location = System::Drawing::Point(x - pic_box->Size.Width / 2, y - pic_box->Size.Height / 2);
}

Transport::Transport(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, System::Windows::Forms::Control^ parent)
{
	this->x = x;
	this->y = y;
	this->isMoving = true;
	this->points_path = create_path(this, departure_point, global_destination_point);
	this->departure_point = departure_point;
	this->destination_point = this->points_path[1];
	this->index = 1;
	this->pic_box = gcnew System::Windows::Forms::PictureBox();
	this->pic_box->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
	this->pic_box->BackColor = System::Drawing::Color::Transparent;
	this->pic_box->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
	this->pic_box->Show();
	parent->Controls->Add(pic_box);
}

Bicycle::Bicycle(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, System::Windows::Forms::Control^ parent) : Transport(x, y, departure_point, global_destination_point, parent)
{
	this->pic_box->Image = System::Drawing::Image::FromFile(delivery::MyForm::path_to_resource + "bicycle_Up.png");
	this->pic_box->Location = System::Drawing::Point(x - 10, y - 20);
	this->pic_box->MaximumSize = System::Drawing::Size(20, 40);
	this->pic_box->Size = System::Drawing::Size(20, 40);
	this->name = "bicycle";
	this->step = 5;
}

Car::Car(int x, int y, MyPoint^ departure_point, MyPoint^ global_destination_point, System::Windows::Forms::Control^ parent) : Transport(x, y, departure_point, global_destination_point, parent)
{
	this->pic_box->Image = System::Drawing::Image::FromFile(delivery::MyForm::path_to_resource + "car_Up.png");
	this->pic_box->Location = System::Drawing::Point(x - 20, y - 40);
	this->pic_box->MaximumSize = System::Drawing::Size(40, 80);
	this->pic_box->Size = System::Drawing::Size(40, 80);
	this->name = "car";
	this->step = 10;
}

void Bicycle::start_event()
{
	if (Store^ st = dynamic_cast<Store^>(destination_point->structure)) LoadEvent(this, destination_point->structure);
	else if (House^ h = dynamic_cast<House^>(destination_point->structure)) UnloadEvent(this, destination_point->structure);
}

void Car::start_event()
{
	if (Warehouse^ wh = dynamic_cast<Warehouse^>(destination_point->structure)) LoadEvent(this, destination_point->structure);
	else if (Store^ st = dynamic_cast<Store^>(destination_point->structure)) UnloadEvent(this, destination_point->structure);
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
	if (target != this) return;

	sender->isMoving = false;

	sender->play_loader_animation(this, sender, 30, 80);

	sender->departure_point = this->point;
	sender->points_path = create_path(sender, sender->departure_point, sender->get_random_store()->car_point);
	sender->destination_point = sender->points_path[1];
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
	if (target != this) return;

	sender->isMoving = false;

	sender->play_loader_animation(this, sender, 20, 40);

	if (Bicycle^ bicycle = dynamic_cast<Bicycle^>(sender))
	{
		System::Random^ rnd = gcnew System::Random();
		System::Collections::Generic::List<MyPoint^>^ all_houses = gcnew System::Collections::Generic::List<MyPoint^>();

		for each (MyPoint^ p in delivery::MyForm::points_bicycle)
		{
			if (p->structure != nullptr && dynamic_cast<House^>(p->structure))
				all_houses->Add(p);
		}

		int idx = rnd->Next(all_houses->Count);
		MyPoint^ house_point = all_houses[idx];

		sender->departure_point = this->bicycle_point;
		sender->points_path = create_path(sender, sender->departure_point, house_point);
		sender->destination_point = sender->points_path[1];
	}
}

void Store::unload(Transport^ sender, Structure^ target)
{
	if (target != this) return;

	sender->isMoving = false;

	sender->play_loader_animation(this, sender, 30, 80);

	sender->departure_point = this->car_point;
	sender->points_path = create_path(sender, sender->departure_point, delivery::MyForm::points_car[0]);
	sender->destination_point = sender->points_path[1];
}

array<System::Tuple<MyPoint^, int>^>^ Store::create_delivery_plan()
{
	System::Random^ rnd = gcnew System::Random();
	System::Collections::Generic::List<MyPoint^>^ all_houses = gcnew System::Collections::Generic::List<MyPoint^>();

	for each (MyPoint ^ p in delivery::MyForm::points_bicycle)
	{
		if (p->structure != nullptr && dynamic_cast<House^>(p->structure))
			all_houses->Add(p);
	}

	List<System::Tuple<MyPoint^, int>^>^ deliveries = gcnew System::Collections::Generic::List<System::Tuple<MyPoint^, int>^>();
	int remaining_capacity = 5;

	while (remaining_capacity > 0 && all_houses->Count > 0)
	{
		int idx = rnd->Next(all_houses->Count);
		MyPoint^ house_point = all_houses[idx];
		all_houses->RemoveAt(idx);
		int volume = rnd->Next(1, Math::Min(3, remaining_capacity) + 1);
		deliveries->Add(gcnew System::Tuple<MyPoint^, int>(house_point, volume));
		remaining_capacity -= volume;
	}

	return deliveries->ToArray();
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
	if (target != this) return;

	sender->isMoving = false;

	sender->play_loader_animation(this, sender, 20, 40);

	Store^ random_store = sender->get_random_store();
	sender->departure_point = this->point;
	sender->points_path = create_path(sender, sender->departure_point, random_store->bicycle_point);
	sender->destination_point = sender->points_path[1];
}

void Transport::log(System::String^ label)
{
	if (Car^ c = dynamic_cast<Car^>(this))
	{
		System::String^ msg =
			"[" + label + "] " + " Departure=" + departure_point->number.ToString() +
			",\tdestination=" + destination_point->number.ToString() +
			",\tcurrent position=(" + x + "," + y + ")" +
			//"), departure=(" + (departure_point != nullptr ? departure_point->x.ToString() : "-1") +
			//"," + (departure_point != nullptr ? departure_point->y.ToString() : "-1") + ")" +
			//", destination=(" + (destination_point != nullptr ? destination_point->x.ToString() : "-1") +
			//"," + (destination_point != nullptr ? destination_point->y.ToString() : "-1") + ")" +
			//", index=" + index +
			//", len=" + (points_path != nullptr ? points_path->Length.ToString() : "-1") +
			//", isMoving=" + (isMoving ? "true" : "false") +
			",\tdirection=" + string(direction);

		System::Diagnostics::Debug::WriteLine(msg);
	}
}

