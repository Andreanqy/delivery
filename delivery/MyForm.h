#pragma once

#include "Source.h"
#include "MyClasses.h"

array<MyPoint^>^ file_to_points(String^);
void file_to_reach_lists(String^, array<MyPoint^>^);

namespace delivery
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public:
		// Заменить
		static System::String^ path_to_resource = "C:\\Users\\Андрей\\Desktop\\ВУЗ\\ИТ\\Марюс\\delivery\\";

		static array<MyPoint^>^ points_car = gcnew array<MyPoint^>(0); // Массив точек маршрутов для машин
		static array<MyPoint^>^ points_bicycle = gcnew array<MyPoint^>(0); // Массив точек маршрутов для велосипедистов

		static array<Structure^>^ structures;
		static array<Transport^>^ transports;
	private: System::Windows::Forms::Timer^ general_timer;

	public:
	private: System::ComponentModel::IContainer^ components;

	public:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
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
		points_bicycle= file_to_points(path_to_resource + "points_bicycle.txt");
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
		Bicycle^ bicycle_1 = gcnew Bicycle(100, 405, points_bicycle[14], points_bicycle[0], this);
		Bicycle^ bicycle_2 = gcnew Bicycle(400, 465, points_bicycle[21], points_bicycle[11], this);

		structures = gcnew array<Structure^> { wh, st1, st2, h1, h2, h3, h4, h5 };
		transports = gcnew array<Transport^> { car, bicycle_1, bicycle_2 };

		for each (Structure^ s in structures)
			for each (Transport^ t in transports)
				s->subscribe_if_relevant(t);

		auto check = transports;
	}
	private: System::Void general_timer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		for each (Transport ^ t in transports)
			t->move();
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		auto check = transports;
	}
};
}
