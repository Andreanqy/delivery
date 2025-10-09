#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::IO;

enum Direction { Up, Down, Left, Right };

ref class MyPoint;

System::String^ string(Direction direction);
array<MyPoint^>^ file_to_points(String^ file_path);
void file_to_reach_lists(String^ file_path, array<MyPoint^>^ points);