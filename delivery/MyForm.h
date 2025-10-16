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

	extern const char* path_to_resource;

	// Класс формы
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MyForm()
		{
			if (components)
				delete components;
		}

	public:
		static bool daytime; // false=ночь, true=день

		static array<MyPoint^>^ points_car = gcnew array<MyPoint^>(0);
		static array<MyPoint^>^ points_bicycle = gcnew array<MyPoint^>(0);

		static array<Structure^>^ structures;
		static array<Transport^>^ transports;

		static System::Collections::Generic::List<LoaderAnimation^>^ active_animations = gcnew System::Collections::Generic::List<LoaderAnimation^>();; // отложенная инициализация active_animations = gcnew List<LoaderAnimation^>();

		static System::String^ path_to_resource = gcnew System::String(delivery::path_to_resource);

	private:
		System::Windows::Forms::Timer^ general_timer;
	private: System::Windows::Forms::Timer^ day_night_timer;

		   System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->general_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->day_night_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// general_timer
			// 
			this->general_timer->Enabled = true;
			this->general_timer->Tick += gcnew System::EventHandler(this, &MyForm::general_timer_Tick);
			// 
			// day_night_timer
			// 
			this->day_night_timer->Interval = 20000;
			this->day_night_timer->Tick += gcnew System::EventHandler(this, &MyForm::day_night_timer_Tick);
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
		daytime = true;

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

		Car^ car = gcnew Car(130, 440, points_car[1], points_car[0], this);
		Bicycle^ bicycle_1 = gcnew Bicycle(100, 405, points_bicycle[14], points_bicycle[1], this);
		bicycle_1->name_number = "bicycle №1";
		Bicycle^ bicycle_2 = gcnew Bicycle(400, 465, points_bicycle[21], points_bicycle[0], this);
		bicycle_2->name_number = "bicycle №2";

		bicycle_1->pic_box->BringToFront();
		bicycle_2->pic_box->BringToFront();

		structures = gcnew array<Structure^> { wh, st1, st2, h1, h2, h3, h4, h5 };
		transports = gcnew array<Transport^> { car, bicycle_1, bicycle_2 };

		for each (Structure ^ s in structures)
			for each (Transport ^ t in transports)
				s->subscribe_if_relevant(t);
	}

	private: System::Void general_timer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		/*
		if (global_time == 300)
		{
			for each (Transport ^ t in transports)
			{
				if (Car^ c = dynamic_cast<Car^>(t))
					c->points_path = create_path(c, c->departure_point, points_car[3]);
				else if (Bicycle^ b = dynamic_cast<Bicycle^>(b))
					b->points_path = create_path(b, b->departure_point, points_bicycle[23]);
			}
		}
		*/

		for each (Transport ^ t in transports)
			t->move();



		for (int i = 0; i < active_animations->Count; i++)
			if (active_animations[i]->update())
				active_animations->RemoveAt(i);
	}
	private: System::Void day_night_timer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		/*
		daytime = !daytime;

		String^ timeName;
		switch (daytime)
		{
		case true: timeName = "День"; break;
		case false: timeName = "Ночь"; break;
		}

		//Console::WriteLine("Наступило: " + timeName);

		for each (Transport ^ t in transports)
		{
			if (!daytime) // ночь
			{
				// Отправляем за пределы карты
				t->go_to_sleep_point();
			}
			else if (daytime) // утро
			{
				// Возвращаем обратно
				t->return_from_sleep_point();
			}
		}
		*/
	}
};
}
