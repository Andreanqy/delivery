#include "MyClasses.h"
#include "MyForm.h"
#include "Source.h"

MyPoint::MyPoint(int num, int x, int y, char type) {
	number = num;
	this->x = x;
	this->y = y;
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
		/*
		isMoving = points_path->Length != ++index;
		if (isMoving) choose_new_destination_point();
		else start_event();
		*/
	}
	print_picture(prev_direction);
}

Store^ Transport::get_random_store()
{
	// Собираем все структуры типа Store
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
	delivery::MyForm::active_animations->Add(gcnew delivery::LoaderAnimation(source, target, size, goal_steps));
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
	if (target != this) return;

	sender->isMoving = false; // остановка транспорта

	// Запускаем анимацию грузчика
	sender->play_loader_animation(this, sender, 20, 40);

	// Если это велосипед — формируем список доставок
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
		sender->direction = (sender->points_path[0]->x > sender->points_path[1]->x) ? Down : Up;
		sender->destination_point = sender->points_path[1];
		int a = 0;
		/*
		// Формируем заказы (каждый — пара <точка_дома, объём>)
		auto plan = create_delivery_plan();
		bicycle->delivery_plan = plan;

		// Подсчёт общего объёма груза
		int total = 0;
		for each (auto t in plan)
			total += t->Item2;
		bicycle->current_load = total;

		bicycle->cargo_index = 0;
		bicycle->points_path = bicycle->create_path(
			bicycle->departure_point,
			plan[0]->Item1
		);

		if (bicycle->points_path->Length > 1)
			bicycle->destination_point = bicycle->points_path[1];
		*/

	}

	// После короткой паузы — включаем движение
	//System::Threading::Tasks::Task::Delay(1000).ContinueWith(gcnew System::Action( [sender]() { sender->isMoving = true; }));
	//System::Threading::Thread::Sleep(1000);
	//sender->isMoving = true;

}


void Store::unload(Transport^ sender, Structure^ target)
{
	if (target == this) { /*Разгрузка машины*/ }
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

	// Анимация разгрузки
	sender->play_loader_animation(this, sender, 20, 40);

	Store^ random_store = sender->get_random_store();
	sender->departure_point = this->point;
	sender->points_path = create_path(sender, sender->departure_point, random_store->bicycle_point);
	sender->destination_point = sender->points_path[1];

	/*
	if (Bicycle^ bicycle = dynamic_cast<Bicycle^>(sender))
	{
		int volume = bicycle->delivery_plan[bicycle->cargo_index]->Item2;
		bicycle->current_load -= volume;

		// Если остались дома — едем к следующему
		bicycle->cargo_index++;

		if (bicycle->cargo_index < bicycle->delivery_plan->Length)
		{
			MyPoint^ next = bicycle->delivery_plan[bicycle->cargo_index]->Item1;
			bicycle->points_path = create_path(
				bicycle->destination_point,
				next
			);

			if (bicycle->points_path->Length > 1)
				bicycle->destination_point = bicycle->points_path[1];
		}
		else
		{
			// Когда всё развёз — возвращается в ближайший магазин
			Store^ random = bicycle->get_random_store(); //get_random_store(bicycle->destination_point);
			if (random != nullptr)
			{
				bicycle->points_path = create_path(
					bicycle->destination_point,
					random->bicycle_point
				);

				if (bicycle->points_path->Length > 1)
					bicycle->destination_point = bicycle->points_path[1];
			}
		}
	}
	*/

	// Возобновляем движение через секунду
	//System::Threading::Tasks::Task::Delay(1000).ContinueWith(gcnew System::Action( [sender]() { sender->isMoving = true; }));
}

void Transport::log(System::String^ label)
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