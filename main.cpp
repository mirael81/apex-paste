#include "main.h"
#include <thread>

void check()
{
	while (TRUE)
	{
		process_id = find_process("r5apex_dx12.exe");
		if (!process_id)
		{
			exit(1);
		}
		std::this_thread::sleep_for(std::chrono::seconds(15));
	}
}

auto init() -> bool
{

	MessageBoxA(0, "Press F1 in lobby", "Cen Apex", MB_OK | MB_ICONINFORMATION);

	if (!find_driver())
	{
		cout << "<!> failed to find driver" << endl;
		return false;
	}

	cout << "press f1 in lobby" << endl;	

	while (true)
	{
		if (GetAsyncKeyState(VK_F1))
		{
			break;
		}

	}

	process_id = find_process("r5apex_dx12.exe");
	if (!process_id)
	{
		cout << "<!> failed to find process" << endl;
		return false;
	}


	auto cr3 = fetch_cr3();
	if (!cr3)
	{
		cout << "<!> Failed to resolve cr3" << endl;
		return false;
	}

	virtualaddy = find_image();
	if (!virtualaddy)
	{
		cout << "<!> Failed to get base address" << endl;
		return false;
	}

	if (!input::init())
	{
		cout << "<!> Failed to Initialize input" << endl;
	}


	bool WindowFocus = false;
	while (WindowFocus == false) {
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (find_process("r5apex_dx12.exe") == ForegroundWindowProcessID) {
			Process.ID = GetCurrentProcessId();
			Process.Handle = GetCurrentProcess();
			Process.Hwnd = GetForegroundWindow();

			RECT TempRect;
			GetWindowRect(Process.Hwnd, &TempRect);
			Width = TempRect.right - TempRect.left;
			Height = TempRect.bottom - TempRect.top;
			Process.WindowLeft = TempRect.left;
			Process.WindowRight = TempRect.right;
			Process.WindowTop = TempRect.top;
			Process.WindowBottom = TempRect.bottom;

			char TempTitle[MAX_PATH];
			GetWindowText(Process.Hwnd, TempTitle, sizeof(TempTitle));
			Process.Title = TempTitle;

			char TempClassName[MAX_PATH];
			GetClassName(Process.Hwnd, TempClassName, sizeof(TempClassName));
			Process.ClassName = TempClassName;

			char TempPath[MAX_PATH];
			GetModuleFileNameEx(Process.Handle, NULL, TempPath, sizeof(TempPath));
			Process.Path = TempPath;

			WindowFocus = true;
		}
	}

	setupoverlay();



	std::thread(setup).detach();
	std::thread(check).detach();
	std::thread(TriggerbotLoop).detach();
	std::thread(superglide).detach();
	std::thread(tapstrafe).detach();
	std::thread(fovchanger).detach();

	while (TRUE)
	{
		render_loop();
	}

}

auto main() -> void
{

	cout << "cen apex cheat" << endl;

	FreeConsole();
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	if (!init())
	{
		cout << "<!> failed to intialize" << endl;
		Sleep(3000);
		exit(0);
	}
}