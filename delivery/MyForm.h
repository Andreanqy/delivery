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

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->SuspendLayout();
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->ClientSize = System::Drawing::Size(547, 524);
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

		Car^ car = gcnew Car(130, 500, points_car[1], points_car[7], this);
		Bicycle^ bicycle_1 = gcnew Bicycle(465, 300, points_bicycle[16], points_bicycle[12], this);
		Bicycle^ bicycle_2 = gcnew Bicycle(100, 250, points_bicycle[13], points_bicycle[11], this);

		array<Structure^>^ structures = { wh, st1, st2, h1, h2, h3, h4, h5 };
		array<Transport^>^ transports = { car, bicycle_1, bicycle_2 };

		for each (Structure^ s in structures)
			for each (Transport^ t in transports)
				s->subscribe_if_relevant(t);
	}
	};
}
