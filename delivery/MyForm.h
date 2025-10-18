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
		static array<Transport^>^ added_transports;

		static System::Collections::Generic::List<LoaderAnimation^>^ active_animations = gcnew System::Collections::Generic::List<LoaderAnimation^>();; // отложенная инициализация active_animations = gcnew List<LoaderAnimation^>();

		static System::String^ path_to_resource = gcnew System::String(delivery::path_to_resource);

	private:
		System::Windows::Forms::Timer^ general_timer;

	private: System::Windows::Forms::CheckedListBox^ checkedListBox1;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Label^ label1;


		   System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		   void InitializeComponent(void)
		   {
			   this->components = (gcnew System::ComponentModel::Container());
			   System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			   this->general_timer = (gcnew System::Windows::Forms::Timer(this->components));
			   this->checkedListBox1 = (gcnew System::Windows::Forms::CheckedListBox());
			   this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			   this->label1 = (gcnew System::Windows::Forms::Label());
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // general_timer
			   // 
			   this->general_timer->Enabled = true;
			   this->general_timer->Tick += gcnew System::EventHandler(this, &MyForm::general_timer_Tick);
			   // 
			   // checkedListBox1
			   // 
			   this->checkedListBox1->FormattingEnabled = true;
			   this->checkedListBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Добавить машину", L"Добавить велосипед" });
			   this->checkedListBox1->Location = System::Drawing::Point(577, 12);
			   this->checkedListBox1->Name = L"checkedListBox1";
			   this->checkedListBox1->Size = System::Drawing::Size(134, 34);
			   this->checkedListBox1->TabIndex = 0;
			   this->checkedListBox1->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &MyForm::checkedListBox1_ItemCheck);
			   // 
			   // trackBar1
			   // 
			   this->trackBar1->Location = System::Drawing::Point(577, 65);
			   this->trackBar1->Maximum = 20;
			   this->trackBar1->Minimum = 1;
			   this->trackBar1->Name = L"trackBar1";
			   this->trackBar1->Size = System::Drawing::Size(134, 45);
			   this->trackBar1->TabIndex = 1;
			   this->trackBar1->Value = 5;
			   this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &MyForm::trackBar1_ValueChanged);
			   // 
			   // label1
			   // 
			   this->label1->AutoSize = true;
			   this->label1->Location = System::Drawing::Point(574, 49);
			   this->label1->Name = L"label1";
			   this->label1->Size = System::Drawing::Size(58, 13);
			   this->label1->TabIndex = 2;
			   this->label1->Text = L"Скорость:";
			   // 
			   // MyForm
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			   this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			   this->ClientSize = System::Drawing::Size(726, 561);
			   this->Controls->Add(this->label1);
			   this->Controls->Add(this->trackBar1);
			   this->Controls->Add(this->checkedListBox1);
			   this->DoubleBuffered = true;
			   this->Name = L"MyForm";
			   this->Text = L"MyForm";
			   this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

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

		Car^ car1 = gcnew Car(130, 440, points_car[1], points_car[0], this);
		Bicycle^ bicycle_1 = gcnew Bicycle(100, 405, points_bicycle[14], points_bicycle[1], this);
		bicycle_1->name_number = "bicycle №1";
		Bicycle^ bicycle_2 = gcnew Bicycle(400, 465, points_bicycle[21], points_bicycle[0], this);
		bicycle_2->name_number = "bicycle №2";

		Car^ car2 = gcnew Car(190, 45, points_car[3], points_car[0], this);
		car2->name_number = "car №2";
		car2->isMoving = false;
		car2->pic_box->Image = System::Drawing::Image::FromFile(delivery::MyForm::path_to_resource + "green_car_Left.png");

		Bicycle^ bicycle_3 = gcnew Bicycle(160, 100, points_bicycle[4], points_bicycle[1], this);
		bicycle_3->name_number = "bicycle №3";
		bicycle_3->isMoving = false;
		bicycle_3->pic_box->Image = System::Drawing::Image::FromFile(delivery::MyForm::path_to_resource + "green_bicycle_Down.png");

		bicycle_1->pic_box->BringToFront();
		bicycle_2->pic_box->BringToFront();
		bicycle_3->pic_box->BringToFront();

		structures = gcnew array<Structure^> { wh, st1, st2, h1, h2, h3, h4, h5 };
		transports = gcnew array<Transport^> { car1, car2, bicycle_1, bicycle_2, bicycle_3 };

		for each (Structure ^ s in structures)
			for each (Transport ^ t in transports)
				s->subscribe_if_relevant(t);
	}

	private: System::Void general_timer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		for each (Transport ^ t in transports)
			t->move();

		for (int i = 0; i < active_animations->Count; i++)
			if (active_animations[i]->update())
				active_animations->RemoveAt(i);
	}
	private: System::Void trackBar1_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		general_timer->Interval = 500 / trackBar1->Value;
	}
	private: System::Void checkedListBox1_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e) {
		String^ itemText = checkedListBox1->Items[e->Index]->ToString();
		if (e->NewValue == System::Windows::Forms::CheckState::Checked)
		{
			if (e->Index == 0) transports[1]->isMoving = true;
			else if (e->Index == 1) transports[4]->isMoving = true;
		}
		else if (e->NewValue == System::Windows::Forms::CheckState::Unchecked)
		{
			if (e->Index == 0)
			{
				Car^ c = dynamic_cast<Car^>(transports[1]);
				c->isMoving = false;
				c->x = 190;
				c->y = 45;
				c->index = 0;
				c->points_path = create_path(points_car[3], points_car[0]);
				c->departure_point = points_car[3];
				c->destination_point = points_car[4];
				c->download_pic->Location = System::Drawing::Point(c->x - 12, c->y - 2);
				c->pic_box->Location = System::Drawing::Point(c->x - c->pic_box->Size.Width / 2, c->y - c->pic_box->Size.Height / 2);
			}
			else if (e->Index == 1)
			{
				Bicycle^ b = dynamic_cast<Bicycle^>(transports[4]);
				b->isMoving = false;
				b->x = 160;
				b->y = 100;
				b->index = 0;
				b->points_path = create_path(points_bicycle[4], points_bicycle[1]);
				b->departure_point = points_car[4];
				b->destination_point = points_car[8];
				b->pic_box->Location = System::Drawing::Point(b->x - b->pic_box->Size.Width / 2, b->y - b->pic_box->Size.Height / 2);
			}
		}
	}
};
}