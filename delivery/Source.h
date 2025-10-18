#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::IO;

enum Direction { Up, Down, Left, Right };

ref class MyPoint;
ref class Transport;

System::String^ string(Direction direction);
array<MyPoint^>^ file_to_points(System::String^ file_path);
void file_to_reach_lists(System::String^ file_path, array<MyPoint^>^ points);
template<typename Mas, typename El> Mas mas_add(Mas old_mas, int& mas_size, El new_element);
array<MyPoint^>^ create_path(MyPoint^ departure_point, MyPoint^ global_destination_point);