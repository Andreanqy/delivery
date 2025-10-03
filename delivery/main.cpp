#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace delivery;

[STAThread]
int Main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm());
    return 0;
}