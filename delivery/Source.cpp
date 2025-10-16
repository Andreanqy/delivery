#include "Source.h"
#include "MyClasses.h"

// Функция, которая преобразует значение Direction в значение System::String
System::String^ string(Direction direction)
{
	switch (direction)
	{
	case Up: return "Up"; break;
	case Down: return "Down"; break;
	case Left: return "Left"; break;
	case Right: return "Right"; break;
	default: return "Up";
	}
}

// Функция, которая добавляет новый элемент в массив
template<typename Mas, typename El> Mas mas_add(Mas old_mas, int& mas_size, El new_element) {
	int n = mas_size + 1;
	Mas new_mas = gcnew array<El>(n);
	for (int i = 0; i < mas_size; i++) new_mas[i] = old_mas[i];
	new_mas[mas_size] = new_element;
	mas_size = n;
	delete old_mas;
	return new_mas;
}

// Функция, которая создает из строки точку
MyPoint^ str_to_point(System::String^ str) {
	//double k = 0.87;
	str += " ";
	int number = -1, x = -1, y = -1, type = -1;
	int count = 0;
	System::String^ temp_str = "";
	for (int i = 0; i < str->Length; i++) {
		if (Char::IsDigit(str[i])) {
			temp_str += str[i];
		}
		else {
			count++;
			if (count == 1 && temp_str != "") number = System::Convert::ToInt32(temp_str);
			else if (count == 2 && temp_str != "") x = System::Convert::ToInt32(temp_str); // * k
			else if (count == 3 && temp_str != "") y = System::Convert::ToInt32(temp_str);
			else if (count == 4 && temp_str != "") type = System::Convert::ToInt32(temp_str);
			else count--;
			temp_str = "";
		}
	}
	return gcnew MyPoint(number, x, y, type);
}

// Функция, которая читает файл и формирует массив точек
array<MyPoint^>^ file_to_points(System::String^ file_path) {
	array<MyPoint^>^ points;
	MyPoint^ temp_point;
	int mas_size = 0;
	StreamReader^ sr = gcnew StreamReader(file_path);
	System::String^ in_buffer = "";
	while (in_buffer = sr->ReadLine()) {
		if (in_buffer->Length != 0) {
			temp_point = str_to_point(in_buffer);
			points = mas_add(points, mas_size, temp_point);
		}
	}
	sr->Close();
	return points;
}

// Функция, которая создает из строки список достижимости
array<int>^ str_to_reach_list(System::String^ str, array<MyPoint^>^ points) {
	str += " ";
	array<int>^ reach_list; // Первый элемент - номер точки, остальные элементы - точки достижимости
	int reach_list_size = 0;
	System::String^ temp_str = "";
	for (int i = 0; i < str->Length; i++) {
		if (Char::IsDigit(str[i])) {
			temp_str += str[i];
		}
		else {
			reach_list = mas_add(reach_list, reach_list_size, System::Convert::ToInt32(temp_str));
			temp_str = "";
		}
	}
	return reach_list;
}

// Функция, которая читает файл и формирует списки достижимости точек
void file_to_reach_lists(System::String^ file_path, array<MyPoint^>^ points) {
	System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(file_path);
	System::String^ in_buffer = "";
	int point_number = -1;
	while (in_buffer = sr->ReadLine()) {
		if (in_buffer->Length != 0) {
			in_buffer += " ";
			array<MyPoint^>^ reach_list;
			int reach_list_size = 0;
			System::String^ temp_str = "";
			int count = 0;
			for (int i = 0; i < in_buffer->Length; i++) {
				if (Char::IsDigit(in_buffer[i])) {
					temp_str += in_buffer[i];
				}
				else {
					if (count == 0 && temp_str != "") {
						point_number = Convert::ToInt32(temp_str);
						count++;
					}
					else if (count > 0 && temp_str != "") {
						reach_list = mas_add(reach_list, reach_list_size, points[Convert::ToInt32(temp_str)]);
						//count++;
					}
					temp_str = "";
				}
			}
			points[point_number]->to_points = reach_list;
		}
	}
	sr->Close();
}

array<MyPoint^>^ create_path(Transport^ t, MyPoint^ departure_point, MyPoint^ global_destination_point)
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

	//t->index = 0;

	return gcnew array<MyPoint^>(0);
}

