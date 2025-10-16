#pragma once

#include "Source.h"
#include "MyClasses.h"
#include <cstdlib>

array<MyPoint^>^ file_to_points(System::String^);
void file_to_reach_lists(System::String^, array<MyPoint^>^);

namespace delivery
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	// Вынесем константу ресурса глобально внутри namespace
	extern const char* path_to_resource;

	// ---------------------------
	// Полное определение LoaderAnimation
	// ---------------------------
	ref class LoaderAnimation
	{
	public:
		PictureBox^ loader;
		int x1, x2, y;
		int step;
		int steps;
		int current;
		int total_steps;
		int goal_steps;
		System::String^ direction;
		Transport^ transport;

		LoaderAnimation(Structure^ source, Transport^ target, int size, int goal_steps)
		{
			if (Bicycle^ b = dynamic_cast<Bicycle^>(target))
			{
				if (Store^ s = dynamic_cast<Store^>(source))
				{
					switch (s->bicycle_point->number)
					{
					case 0: x1 = s->bicycle_point->x + 40; x2 = target->x; break;
					case 1: x1 = s->bicycle_point->x - 60; x2 = target->x - 15; break;
					}
				}
				else if (House^ h = dynamic_cast<House^>(source))
				{
					switch (h->point->number)
					{
					case 2:
					case 11:
					case 13: x1 = h->point->x - 60; x2 = target->x - 15; break;
					case 12:
					case 16: x1 = h->point->x + 60; x2 = target->x + 15; break;
					}
				}
			}
			else if (Car^ c = dynamic_cast<Car^>(target))
			{
				if (Store^ s = dynamic_cast<Store^>(target))
				{
					switch (s->car_point->number)
					{
					case 0: x1 = s->car_point->x + 60; x2 = target->x + 15; break;
					case 1: x1 = s->car_point->x - 60; x2 = target->x - 15; break;
					}
				}
				if (Warehouse^ wh = dynamic_cast<Warehouse^>(source))
				{
					x1 = wh->point->x - 90;
				}
			}

			y = target->y;
			step = 5;
			steps = abs(x2 - x1) / step;
			current = 0;
			total_steps = 0;
			this->goal_steps = goal_steps;
			transport = target;

			// Определяем направление для картинки
			direction = (x2 - x1 > 0) ? "Right" : "Left";

			// Создаём PictureBox
			System::Windows::Forms::Control^ parent = target->pic_box->Parent;
			loader = gcnew System::Windows::Forms::PictureBox();
			loader->Image = System::Drawing::Image::FromFile(gcnew System::String(path_to_resource) + "loader_" + direction + ".png");
			loader->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			loader->BackColor = System::Drawing::Color::Transparent;
			loader->Size = System::Drawing::Size(size, size);
			loader->Location = System::Drawing::Point(x1, y);
			parent->Controls->Add(loader);
			loader->BringToFront();
		}

		// Возвращает true, если анимация завершена
		/*bool Update()
		{
			if (forward)
			{
				loader->Location = Point(x1 + (direction == "Right" ? step : -step), y);
				if (current >= steps)
				{
					forward = false;
					current = 0;
				}
			}
			else
			{
				loader->Location = Point(x2 - (direction == "Right" ? step : -step), y);
				current++;
				if (current >= steps)
				{
					loader->Parent->Controls->Remove(loader);
					delete loader;
					loader = nullptr;
					return true;
				}
			}
			return false;
		}*/

		bool update()
		{
			loader->Location = System::Drawing::Point(loader->Location.X + (direction == "Right" ? step : -step), y - (loader->Size.Height/2));
			current++;
			total_steps++;
			if (current >= steps)
			{
				direction = (direction == "Right") ? "Left" : "Right";
				loader->Image = System::Drawing::Image::FromFile(gcnew System::String(path_to_resource) + "loader_" + direction + ".png");
				current = 0;
			}

			if (total_steps == goal_steps) // например, добавь флаг forward, как в прошлой версии
			{
				loader->Parent->Controls->Remove(loader);
				delete loader;
				loader = nullptr;
				transport->isMoving = true;
				return true; // сигнал — удалить из списка
			}

			return false;
		}
	};

	// ---------------------------
	// Класс формы
	// ---------------------------
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

			// Инициализация статического списка после полного определения LoaderAnimation
			//if (active_animations == nullptr) active_animations = gcnew List<LoaderAnimation^>();
		}

	protected:
		~MyForm()
		{
			if (components)
				delete components;
		}

	public:
		static array<MyPoint^>^ points_car = gcnew array<MyPoint^>(0);
		static array<MyPoint^>^ points_bicycle = gcnew array<MyPoint^>(0);

		static array<Structure^>^ structures;
		static array<Transport^>^ transports;

		static System::Collections::Generic::List<LoaderAnimation^>^ active_animations = gcnew System::Collections::Generic::List<LoaderAnimation^>();; // отложенная инициализация active_animations = gcnew List<LoaderAnimation^>();

		static System::String^ path_to_resource = gcnew System::String(delivery::path_to_resource);

	private:
		System::Windows::Forms::Timer^ general_timer;
		System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->general_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// general_timer
			// 
			this->general_timer->Enabled = true;
			this->general_timer->Tick += gcnew System::EventHandler(this, &MyForm::general_timer_Tick);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ClientSize = System::Drawing::Size(564, 561);
			this->DoubleBuffered = true;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
		}
#pragma endregion

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		points_car = file_to_points(path_to_resource + "points_car.txt");
		file_to_reach_lists(path_to_resource + "reach_lists_car.txt", points_car);
		points_bicycle = file_to_points(path_to_resource + "points_bicycle.txt");
		file_to_reach_lists(path_to_resource + "reach_lists_bicycle.txt", points_bicycle);

		Warehouse^ wh = gcnew Warehouse(0);
		Store^ st1 = gcnew Store(0, 2);
		Store^ st2 = gcnew Store(1, 1);
		House^ h1 = gcnew House(2);
		House^ h2 = gcnew House(11);
		House^ h3 = gcnew House(13);
		House^ h4 = gcnew House(12);
		House^ h5 = gcnew House(16);

		Car^ car = gcnew Car(130, 440, points_car[7], points_car[2], this);
		Bicycle^ bicycle_1 = gcnew Bicycle(100, 405, points_bicycle[14], points_bicycle[1], this);
		bicycle_1->name_number = "bicycle №1";
		Bicycle^ bicycle_2 = gcnew Bicycle(400, 465, points_bicycle[21], points_bicycle[0], this);
		bicycle_2->name_number = "bicycle №2";

		structures = gcnew array<Structure^> { wh, st1, st2, h1, h2, h3, h4, h5 };
		transports = gcnew array<Transport^> { bicycle_1}; // car, bicycle_2 

		for each (Structure ^ s in structures)
			for each (Transport ^ t in transports)
				s->subscribe_if_relevant(t);
	}

	private: System::Void general_timer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		for each (Transport ^ t in transports)
			t->move();

		for (int i = 0; i < active_animations->Count; i++)
			//active_animations[i]->update();
			if (active_animations[i]->update())
				active_animations->RemoveAt(i); // теперь работает
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		auto check = transports;
	}
	};
}
