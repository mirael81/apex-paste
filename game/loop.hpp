#include "../main.h"
#include "offsets.hpp"
#include <random>

static int keystatus = 0;
static int keystatus2 = 0;
static int keystatus3 = 0;
static int keystatus4 = 0;
namespace hotkeys
{
	int aimkey;
	int triggerkey;
	int thirdpersonkey;
}

struct Snowflake {
	float x, y;         // Position of the snowflake
	float speed;        // Speed of the snowflake falling
	float size;         // Size of the snowflake
};


void render_snowflakes(bool menuVisible)
{
	static std::vector<Snowflake> snowflakes;
	static std::default_random_engine generator;
	static std::uniform_real_distribution<float> distributionX(0.0f, ImGui::GetIO().DisplaySize.x);
	static std::uniform_real_distribution<float> distributionSize(2.0f, 5.0f);
	static std::uniform_real_distribution<float> distributionSpeed(0.1f, 1.0f);

	if (menuVisible) {

		if (snowflakes.size() < 200) {
			snowflakes.push_back(Snowflake{
				distributionX(generator),
				0.0f,
				distributionSpeed(generator),
				distributionSize(generator)
				});
		}


		for (auto& snowflake : snowflakes) {
			snowflake.y += snowflake.speed;
			if (snowflake.y > ImGui::GetIO().DisplaySize.y) {
				snowflake.y = 0.0f;
			}

			// Draw snowflake as a small circle
			ImGui::GetForegroundDrawList()->AddCircleFilled(
				ImVec2(snowflake.x, snowflake.y),
				snowflake.size,
				IM_COL32(255, 255, 255, 255)
			);
		}
	}
	else {

		snowflakes.clear();
	}
}


bool ColorBarEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 size = ImGui::CalcItemSize(size_arg, 100.0f, 6.0f); // default size if none provided
	ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y); // correct addition
	ImRect bb(pos, max);

	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	// Draw colorful gradient bar
	ImU32 black = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.43f));

	ImVec2 p = bb.Min;
	float half_width = size.x / 2.0f;

	// Left gradient
	window->DrawList->AddRectFilledMultiColor(
		ImVec2(p.x, p.y),
		ImVec2(p.x + half_width, p.y + size.y),
		ImColor(55, 177, 218),
		ImColor(201, 84, 192),
		ImColor(201, 84, 192),
		ImColor(55, 177, 218)
	);

	// Right gradient
	window->DrawList->AddRectFilledMultiColor(
		ImVec2(p.x + half_width, p.y),
		ImVec2(p.x + size.x, p.y + size.y),
		ImColor(201, 84, 192),
		ImColor(204, 227, 54),
		ImColor(204, 227, 54),
		ImColor(201, 84, 192)
	);

	// Optional black overlay for shading
	window->DrawList->AddRectFilled(bb.Min, bb.Max, black);

	return false; // This function is not interactive
}

bool ColorBar(const char* label, const ImVec2& size_arg)
{
	return ColorBarEx(label, size_arg, 0);
}


static const char* keyNames[] =
{
	(""),
	("Left Mouse"),
	("Right Mouse"),
	("Cancel"),
	("Middle Mouse"),
	("Mouse 5"),
	("Mouse 4"),
	(""),
	("Backspace"),
	("Tab"),
	(""),
	(""),
	("Clear"),
	("Enter"),
	(""),
	(""),
	("Shift"),
	("Control"),
	("Alt"),
	("Pause"),
	("Caps"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Escape"),
	(""),
	(""),
	(""),
	(""),
	("Space"),
	("Page Up"),
	("Page Down"),
	("End"),
	("Home"),
	("Left"),
	("Up"),
	("Right"),
	("Down"),
	(""),
	(""),
	(""),
	("Print"),
	("Insert"),
	("Delete"),
	(""),
	("0"),
	("1"),
	("2"),
	("3"),
	("4"),
	("5"),
	("6"),
	("7"),
	("8"),
	("9"),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	(""),
	("A"),
	("B"),
	("C"),
	("D"),
	("E"),
	("F"),
	("G"),
	("H"),
	("I"),
	("J"),
	("K"),
	("L"),
	("M"),
	("N"),
	("O"),
	("P"),
	("Q"),
	("R"),
	("S"),
	("T"),
	("U"),
	("V"),
	("W"),
	("X"),
	("Y"),
	("Z"),
	(""),
	(""),
	(""),
	(""),
	(""),
	("Numpad 0"),
	("Numpad 1"),
	("Numpad 2"),
	("Numpad 3"),
	("Numpad 4"),
	("Numpad 5"),
	("Numpad 6"),
	("Numpad 7"),
	("Numpad 8"),
	("Numpad 9"),
	("Multiply"),
	("Add"),
	(""),
	("Subtract"),
	("Decimal"),
	("Divide"),
	("F1"),
	("F2"),
	("F3"),
	("F4"),
	("F5"),
	("F6"),
	("F7"),
	("F8"),
	("F9"),
	("F10"),
	("F11"),
	("F12")
};
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys;
	if (preview_value == NULL)
		aimkeys = ("Select Key");
	else
		aimkeys = preview_value;

	if (status == 1)
	{
		aimkeys = ("Press the key");
	}
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 25)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

void Hotkeytrigger(int triggerkey, void* changekey, int status, const char* label)
{
	const char* preview_value = NULL;
	if (triggerkey >= 0 && triggerkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, triggerkey, &preview_value);

	std::string triggerkeys;
	if (preview_value == NULL)
		triggerkeys = ("Select Key");
	else
		triggerkeys = preview_value;

	if (status == 1)
	{
		triggerkeys = ("Press the key");
	}
	if (ImGui::Button(triggerkeys.c_str(), ImVec2(125, 25)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
	if (label)
		ImGui::SameLine();
	ImGui::Text("%s", label);
}

void HotkeyThirdperson(int thirdpersonkey, void* changekey, int status)
{
	const char* preview_value = NULL;
	if (thirdpersonkey >= 0 && thirdpersonkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, thirdpersonkey, &preview_value);
	std::string thirdpersonkeys;
	if (preview_value == NULL)
		thirdpersonkeys = ("Select Key");
	else
		thirdpersonkeys = preview_value;
	if (status == 1)
	{
		thirdpersonkeys = ("Press the key");
	}
	if (ImGui::Button(thirdpersonkeys.c_str(), ImVec2(125, 25)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
}

namespace settings
{
	bool menu = true;
}

auto InputHandler() -> void {
	for (int i = 0; i < 5; i++) {
		ImGui::GetIO().MouseDown[i] = false;
	}

	int Button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) {
		Button = 0;
	}

	if (Button != -1) {
		ImGui::GetIO().MouseDown[Button] = true;
	}
}

namespace settings
{
	bool aimbot = false;
	bool showfov = false;
	bool ragefov = false;
	bool vischeck = true;
	bool skeleton = false;
	int fov = 0;
	float smoothing = 1.f;
	bool box = false;
	float boxcolor
	[3] = {255, 255, 255};
	bool name = false;
	bool headdot = false;
	bool health = false;
	bool teamcheck = true;
	bool sheild = false;
	bool streamproof = true;
	int key = 0x02;
	bool tdmenu = false;
	
	bool engine = true;
	bool glow = false;
	bool viewmodelglow = false;
	bool noviewmodel = false;
	bool localmodelchams = false;
	bool nosky = false;
	bool third_person = false;
	int keybind3rd;
	bool fullbright = false;
	bool handglow = false;
	bool fakefire = false;
	bool unlockall = false;
	bool skinchanger = false;
	int skinid;

	bool fovchanger = false;
	bool freecam = false;
	float desirefov = 110;

	bool crosshair = false;
	bool superglide;
	bool tapstrafe;

	bool playerskinchanger = false;
	int playerskinid;

	bool knifechanger = false;
	int  modelid = 1;

	bool gunchanger = false;
	int guntype = 1;
	int gunid = 1;

	bool antiaim = false;

	bool triggerbot = false;
	int ms;
	int keybind = 0x05;

	int superbind = 0x05;

	enum pitch
	{
		up,
		down,
		zero
	};

	enum yaw
	{
		left,
		right,
		back,
		zeroo,
		spin,
		jitter
	};

	pitch Pitch = down;
	yaw Yaw = back;

	float rgb[3] = { 255, 255, 255 };
	float rgbI[3] = { 255, 255, 255 };

	enum AimPart
	{
		HEAD,
		PELVIS
	};

	AimPart aimpart = HEAD;
}

void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				settings::key = i;
				keystatus = 0;
				return;
			}
		}
	}
}

void ChangeKey2(void* blank)
{
	keystatus2 = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				settings::keybind = i;
				keystatus2 = 0;
				return;
			}
		}
	}
}

void ChangeKey3(void* blank)
{
	keystatus3 = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				settings::keybind3rd = i;
				keystatus3 = 0;
				return;
			}
		}
	}
}

void ChangeKey4(void* blank)
{
	keystatus4 = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				settings::superbind = i;
				keystatus4 = 0;
				return;
			}
		}
	}
}


struct WP_HighlightFunctionBits
{
	uint8_t functionBits[4];
};

struct WP_HighlightParameter
{
	float parameter[4];
};


void aimbot(int x, int y)
{


	float centerX = Width / 2.0f;
	float centerY = Height / 2.0f;

	float dx = x - centerX;
	float dy = y - centerY;

	const float deadzone = 0.00f;

	if (fabsf(dx) < deadzone && fabsf(dy) < deadzone)
		return;

	dx /= settings::smoothing;
	dy /= settings::smoothing;

	input::move_mouse(dx, dy);
}


void drawmenu()
{
	// Static tab index for tracking active page
	static int activeTab = 0;

	

	// Sidebar dimensions
	ImVec2 sidebar_size = ImVec2(120, 0); // width of sidebar
	ImVec2 content_size = ImVec2(0, 0);   // fill rest of window



	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(6, 6);

	ImGui::SetNextWindowSize(ImVec2(660.f, 560.f));
	if (GetAsyncKeyState(VK_INSERT) & 1) settings::menu = !settings::menu;
	ImGui::Begin("eax (by cen)", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
	{
		render_snowflakes(settings::menu);
		ImGui::BeginChild("Complete Border", ImVec2(648.f, 548.f), false); {
			ImVec2 window_pos = ImGui::GetWindowPos();
			ImVec2 window_size = ImGui::GetWindowSize();

			ImVec2 image_size = ImVec2(648.f, 548.f);

			// Calculate top-left position to center the image inside the window
			ImVec2 image_pos;
			image_pos.x = window_pos.x + (window_size.x - image_size.x) * 0.5f;
			image_pos.y = window_pos.y + (window_size.y - image_size.y) * 0.5f;

			// Draw the image in window’s draw list (so it respects clipping inside the window)
			ImGui::GetWindowDrawList()->AddImage(bg, image_pos, ImVec2(image_pos.x + image_size.x, image_pos.y + image_size.y));

		} ImGui::EndChild();

		ImGui::SameLine(6.f);

		style->Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0, 0);

		int tab = 0;

		style->ItemSpacing = ImVec2(0.f, -0.f);

		ImGui::BeginChild("Menu Contents", ImVec2(648.f, 548.f), false); {
			ColorBar("unicorn", ImVec2(648.f, 2.f));
			ImGui::Columns(2, nullptr, true);
			ImGui::SetColumnWidth(0, 78.f);
			style->ItemSpacing = ImVec2(0.f, -1.f);

			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImVec2 size = ImVec2(75, 548.f);

			ImU32 color = IM_COL32(12, 12, 12, 255);
			ImVec2 shifted_pos = ImVec2(pos.x, pos.y + 1);
			draw_list->AddRectFilled(shifted_pos, ImVec2(shifted_pos.x + size.x, shifted_pos.y + size.y), color);

			ImVec2 cursor_pos = ImGui::GetCursorPos();

			ImGui::PushFont(sidefont);

			// Button A
			{
				ImVec2 btn_pos = cursor_pos;
				btn_pos.y += 2;
				btn_pos.x -= 2;
				ImGui::SetCursorPos(btn_pos);
				if (ImGui::Button("A", ImVec2(75.f, 75.f))) activeTab = 0;
			}

			// Button B
			{
				ImVec2 btn_pos = cursor_pos;
				btn_pos.y += 79;  // 75 height + 4 padding (adjust as needed)
				btn_pos.x -= 2;
				ImGui::SetCursorPos(btn_pos);
				if (ImGui::Button("D", ImVec2(75.f, 75.f))) activeTab = 1;
			}

			// Button C
			{
				ImVec2 btn_pos = cursor_pos;
				btn_pos.y += 156; // 75*2 + 6 padding approx
				btn_pos.x -= 2;
				ImGui::SetCursorPos(btn_pos);
				if (ImGui::Button("B", ImVec2(75.f, 75.f))) activeTab = 2;
			}

			// Button D
			{
				ImVec2 btn_pos = cursor_pos;
				btn_pos.y += 233; // 75*3 + 8 padding approx
				btn_pos.x -= 2;
				ImGui::SetCursorPos(btn_pos);
				if (ImGui::Button("I", ImVec2(75.f, 75.f))) activeTab = 3;
			}

			// Button E
			{
				ImVec2 btn_pos = cursor_pos;
				btn_pos.y += 310; // 75*4 + 10 padding approx
				btn_pos.x -= 2;
				ImGui::SetCursorPos(btn_pos);
				if (ImGui::Button("G", ImVec2(75.f, 75.f))) activeTab = 4;
			}

			ImGui::PopFont();

			ImGui::NextColumn();


			ImGui::BeginChild("MainContent");
			{

				style->ItemSpacing = ImVec2(8, 4);
				if (activeTab == 0)
				{
					ImGui::Text("eax by cen");
					ImGui::Checkbox("Enabled", &settings::aimbot);
					ImGui::Checkbox("Visible Only", &settings::vischeck);
					ImGui::Checkbox("360 fov", &settings::ragefov);
					ImGui::Checkbox("Show FOV", &settings::showfov);
					ImGui::SliderInt("FOV", &settings::fov, 0, 500);
					ImGui::SliderFloat("Smoothing", &settings::smoothing, 1.0f, 50.f);

					const char* parts[] = { "Head", "Pelvis" };
					ImGui::Combo("Aim Part", (int*)&settings::aimpart, parts, IM_ARRAYSIZE(parts));
					HotkeyButton(settings::key, ChangeKey, keystatus);

					ImGui::Spacing();	
					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Checkbox("Triggerbot", &settings::triggerbot);
					ImGui::SliderInt("Delay (ms)", &settings::ms, 0, 100);
					HotkeyButton(settings::keybind, ChangeKey2, keystatus2);

				}
				else if (activeTab == 1)
				{
					ImGui::Text("eax by cen");
					ImGui::Checkbox("Box", &settings::box); ImGui::SameLine(); ImGui::ColorEdit3("##Box Color", settings::boxcolor, ImGuiColorEditFlags_NoInputs);
					ImGui::Checkbox("Name", &settings::name);
					ImGui::Checkbox("Head Dot", &settings::headdot);
					ImGui::Checkbox("Health Bar", &settings::health);
					ImGui::Checkbox("Shield Bar", &settings::sheild);
					ImGui::Checkbox("Team check", &settings::teamcheck);
				}
				else if (activeTab == 2)
				{
					ImGui::Text("eax by cen");
					ImGui::Checkbox("Skin Changer", &settings::skinchanger);
					if (settings::skinchanger)
						ImGui::SliderInt("Skin ID", &settings::skinid, 0, 25);

					ImGui::Checkbox("Heirloom Changer", &settings::knifechanger);
					if (settings::knifechanger)
						ImGui::SliderInt("Model ID", &settings::modelid, 1, 25);

					ImGui::Checkbox("Gun Changer", &settings::gunchanger);
					if (settings::gunchanger)
					{
						ImGui::SliderInt("Gun ID", &settings::gunid, 1, 5);
						ImGui::SliderInt("Gun type", &settings::guntype, 1, 2);
					}

					if (ImGui::Button("Unlock All", ImVec2(125, 25)))
						write<DWORD64>(virtualaddy + 0x026fa860 + 0x64, 1);
				}
				else if (activeTab == 3)
				{
					ImGui::Text("eax by cen");
					ImGui::Checkbox("Enable Anti-aim", &settings::antiaim);
					if (settings::antiaim)
					{
						ImGui::Text("Pitch");
						const char* pitchOptions[] = { "Up", "Down", "Zero" };
						ImGui::Combo("##Pitch", (int*)&settings::Pitch, pitchOptions, IM_ARRAYSIZE(pitchOptions));

						ImGui::Text("Yaw");
						const char* yawOptions[] = { "Left", "Right", "Back", "zeroo", "Spin", "Jitter" };
						ImGui::Combo("##Yaw", (int*)&settings::Yaw, yawOptions, IM_ARRAYSIZE(yawOptions));
					}
				}
				else if (activeTab == 4)
				{
					ImGui::Text("eax by cen");
					ImGui::Checkbox("Enable Engine", &settings::engine);
					if (settings::engine)
					{
						ImGui::Checkbox("Chams", &settings::glow);
						ImGui::Checkbox("Weapon Chams", &settings::viewmodelglow);
						ImGui::Checkbox("Hand Chams", &settings::handglow);
						ImGui::Checkbox("Local Chams", &settings::localmodelchams);
						ImGui::Checkbox("No Viewmodel", &settings::noviewmodel);
						ImGui::Checkbox("No Sky", &settings::nosky);
						ImGui::Checkbox("Full Bright", &settings::fullbright);
						ImGui::Checkbox("FOV Changer", &settings::fovchanger);
						if (settings::fovchanger)
							ImGui::SliderFloat("##", &settings::desirefov, 0.f, 180.f);

						ImGui::Checkbox("fake fire", &settings::fakefire);

						ImGui::Checkbox("Super Glide", &settings::superglide);
						if (settings::superglide)
						{
							ImGui::Text("Super Glide Key");
							HotkeyButton(settings::superbind, ChangeKey4, keystatus4);
						}

						ImGui::Checkbox("Tap Strafe", &settings::tapstrafe);

						ImGui::Checkbox("Third Person", &settings::third_person);
						if (settings::third_person)
						{
							ImGui::Text("Third Person Key");
							HotkeyThirdperson(settings::keybind3rd, ChangeKey3, keystatus3);
						}
					}
					ImGui::Checkbox("Crosshair", &settings::crosshair);
					if (ImGui::Button("Exit"))
						exit(0);
				}

				ImVec2 childPos = ImGui::GetCursorScreenPos(); // Top-left of the child
				ImVec2 childSize = ImGui::GetContentRegionAvail(); // Size of the available region inside the child
				ImVec2 imageSize = ImVec2(100, 150); // Replace with your actual image size

				// Move cursor to the bottom-right corner minus the image size
				ImGui::SetCursorScreenPos(ImVec2(
					childPos.x + childSize.x - imageSize.x,
					childPos.y + childSize.y - imageSize.y
				));
				ImGui::Image(cleo, imageSize);
			}
			ImGui::EndChild();

		}
	}
	ImGui::End();

}

inline std::vector<c_player> player_list;

bool loopae;

void heirloom()
{
	while (settings::knifechanger)
	{

		if (settings::gunchanger)
		{
			c_player localplayerr = read<uint64_t>(virtualaddy + OFF_LOCAL_PLAYER);
			auto view_model_handle = read<uint64_t>(localplayerr.get_address() + OFF_VIEWMODEL); //m_hViewModels
			view_model_handle &= 0xFFFF;
			auto view_model_ptr = read<uint64_t>(virtualaddy + OFF_ENTITY_LIST + (view_model_handle << 5));

			char modelName[200] = { 0 };
			auto name_ptr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
			ReadArray<char>(name_ptr, modelName, 200);
			std::string model_name_str = std::string(modelName);

			int weapond = localplayerr.weapon().weapon_id();


			if (settings::guntype == 1)
			{
				if (settings::gunid == 1)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/Weapons/r97/ptpov_r99_s07_reactive.rmdl");
					write<int>(view_model_ptr + 0xD8, 4159);

					if (model_name_str.find("ptpov_octane_knife") != std::string::npos) { // 是否为传家宝
						int cur_sequence, modelAniIndex;
						cur_sequence = read<int>(view_model_ptr + 0xe18);
						modelAniIndex = read<int>(view_model_ptr + 0xe10);
						if (cur_sequence == 0 && modelAniIndex == 4614) {
							int flags = read<int>(localplayerr.get_address() + 0x00c8);
							int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
							int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
							int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
							int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
							int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
							int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
							int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
							int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

							int seq = 1; // Default state: idle

							// Determine movement/action state
							if (flags == 65)					 seq = 1;  // On ground
							if (flags == 64)					 seq = 4;
							if (in_duck)                         seq = 2;  // Ducking
							if (in_forward)                      seq = 82;  // Forward
							if (in_backward)                     seq = 81;  // Backward
							if (in_left)                         seq = 83;  // Left
							if (in_right)                        seq = 84;  // Right
							if (in_attack == 5)                 seq = 22; // Shooting
							if (in_speed == 5)                  seq = 5; // Sprinting

							write(view_model_ptr + 0xe18, seq);
						}
					}	
				}
				else if (settings::gunid == 2)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/Weapons/r97/ptpov_r99_s04_bp.rmdl");
					write<int>(view_model_ptr + 0xD8, 4155);
				}
				else if (settings::gunid == 3)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/Weapons/r97/ptpov_r97_legendary_05.rmdl");
					write<int>(view_model_ptr + 0xD8, 4149);
				}
				else if (settings::gunid == 4)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "dl/Weapons/r97/ptpov_r99_legendary_06.rmdl");
					write<int>(view_model_ptr + 0xD8, 4151);
				}
				else if (settings::gunid == 5)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "ptpov_r97_s08e01_cevent_legendary_01.rmdl");
					write<int>(view_model_ptr + 0xD8, 4161);
				}
			}
			if (settings::guntype == 2)
			{
				if (settings::gunid == 1)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/techart/mshop/weapons/class/assault/flatline/flatline_v19_space_v.rmdl");
					write<int>(view_model_ptr + 0xD8, 3786);
				}
				else if (settings::gunid == 2)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/techart/mshop/weapons/class/assault/flatline/flatline_v20_champion_v.rmdl");
					write<int>(view_model_ptr + 0xD8, 3796);
				}
				else if (settings::gunid == 3)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/techart/mshop/weapons/class/assault/flatline/flatline_lgnd_v22_hunterpredator_v.rmdl");
					write<int>(view_model_ptr + 0xD8, 3808);
				}
				else if (settings::gunid == 4)
				{
					uint64_t modelNamePtr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
					write<const char*>(modelNamePtr, "mdl/techart/mshop/weapons/class/assault/flatline/flatline_v20_assim_v.rmdl");
					write<int>(view_model_ptr + 0xD8, 3792);
				}
			}
			
		}


		if (settings::knifechanger)
		{


			uint64_t localplayerr = read<uint64_t>(virtualaddy + OFF_LOCAL_PLAYER);
			auto view_model_handle = read<uint64_t>(localplayerr + OFF_VIEWMODEL); //m_hViewModels
			view_model_handle &= 0xFFFF;
			auto view_model_ptr = read<uint64_t>(virtualaddy + OFF_ENTITY_LIST + (view_model_handle << 5));

			char modelName[200] = { 0 };
			auto name_ptr = read<uint64_t>(view_model_ptr + OFF_MODELNAME);
			ReadArray<char>(name_ptr, modelName, 200);
			std::string model_name_str = std::string(modelName);


			int cur_sequence, modelAniIndex;

			if (settings::modelid == 1)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/Weapons/octane_knife/ptpov_octane_knife.rmdl");
					write(view_model_ptr + 0xd8, 4614);
				}
				else if (model_name_str.find("ptpov_octane_knife") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4614) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting

						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
			else if (settings::modelid == 2)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_cosmicmerc/heirloom_karambit_v24_cosmicmerc_v.rmdl");
					write(view_model_ptr + 0xd8, 4694);
				}
				else if (model_name_str.find("heirloom_karambit_v24_cosmicmerc_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4694) {

						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting

						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
			else if (settings::modelid == 3)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_riseego/heirloom_agnostic_v24_riseego_v.rmdl");
					write(view_model_ptr + 0xd8, 4685);
				}

				else if (model_name_str.find("heirloom_agnostic_v24_riseego_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4685) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting

						write(view_model_ptr + 0xe18, seq);
					}
				}

			}
			else if (settings::modelid == 4)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/legends/v18_kunai/heirloom_legends_v18_kunai_wraith_rt01_v.rmdl");
					write(view_model_ptr + 0xd8, 4657);
				}
				else if (model_name_str.find("heirloom_legends_v18_kunai_wraith_rt01_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4657) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting
						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
			else if (settings::modelid == 5)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bangalore/v20_khukuri/heirloom_bangalore_v20_khukuri_v.rmdl");
					write(view_model_ptr + 0xd8, 4598);
				}
				else if (model_name_str.find("heirloom_bangalore_v20_khukuri_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4598) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting
						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
			else if (settings::modelid == 6)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/agnostic/v24_maniaworld/heirloom_hooksword_v24_maniaworld_v.rmdl");
					write(view_model_ptr + 0xd8, 4690);
				}
				else if (model_name_str.find("heirloom_hooksword_v24_maniaworld_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 4690) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 82;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 22; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting
						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
			else if (settings::modelid == 7)
			{
				if (model_name_str.find("emptyhand") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("empty_handed") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("empt") != std::string::npos) { // 116挥拳 是否空手
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("░▀eN≈⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("░▀") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("±2▀÷⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("⌂") != std::string::npos) {
					write<const char*>(name_ptr, "mdl/techart/mshop/weapons/class/heirloom/bloodhound/v19_axe/heirloom_bloodhound_v19_axe_v.rmdl");
					write(view_model_ptr + 0xd8, 302);
				}
				else if (model_name_str.find("heirloom_bloodhound_v19_axe_v.rmdl") != std::string::npos) { // 是否为传家宝
					// 获得动画序列
					cur_sequence = read<int>(view_model_ptr + 0xe18);
					modelAniIndex = read<int>(view_model_ptr + 0xe10);
					if (cur_sequence == 0 && modelAniIndex == 302) {
						int flags = read<int>(localplayerr + 0x00c8);
						int in_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
						int in_backward = read<int>(virtualaddy + OFF_IN_BACKWARD + 0x8);
						int in_left = read<int>(virtualaddy + OFF_IN_MOVELEFT + 0x8);
						int in_right = read<int>(virtualaddy + OFF_IN_MOVERIGHT + 0x8);
						int in_jump = read<int>(virtualaddy + OFF_IN_JUMP + 0x8);
						int in_duck = read<int>(virtualaddy + OFF_IN_DUCK + 0x8);
						int in_attack = read<int>(virtualaddy + OFF_INATTACK + 0x8);
						int in_speed = read<int>(virtualaddy + 0x073e0b60 + 0x8);

						int seq = 1; // Default state: idle

						// Determine movement/action state
						if (flags == 65)					 seq = 1;  // On ground
						if (flags == 64)					 seq = 4;
						if (in_duck)                         seq = 2;  // Ducking
						if (in_forward)                      seq = 1;  // Forward
						if (in_backward)                     seq = 81;  // Backward
						if (in_left)                         seq = 83;  // Left
						if (in_right)                        seq = 84;  // Right
						if (in_attack == 5)                 seq = 82; // Shooting
						if (in_speed == 5)                  seq = 5; // Sprinting
						write(view_model_ptr + 0xe18, seq);
					}
				}
			}
		}

	}
	loopae = false;

}

c_vec3 calculate_antiaim_angles(settings::pitch Pitch, settings::yaw Yaw)
{
	float pitch_angle = 0.f;
	float yaw_angle = 0.f;

	// Handle pitch
	switch (Pitch)
	{
	case settings::up:    pitch_angle = -120.f; break;
	case settings::down:  pitch_angle = 120.f; break;
	case settings::zero:  pitch_angle = 0.f; break;
	}

	// Handle yaw
	static float spin_yaw = 0.f;
	switch (Yaw)
	{
	case settings::left:     yaw_angle = 90.f; break;
	case settings::right:    yaw_angle = -90.f; break;
	case settings::back:     yaw_angle = 180.f; break;
	case settings::zeroo:     yaw_angle = 0.f; break;
	case settings::spin:
		spin_yaw += 10.f;
		if (spin_yaw > 360.f) spin_yaw -= 360.f;
		yaw_angle = spin_yaw;
		break;
	case settings::jitter:
		static bool flip = false;
		yaw_angle = flip ? 68.f : -68.f;
		flip = !flip;
		break;
	}

	return c_vec3(yaw_angle, pitch_angle, yaw_angle);
}

namespace trigger
{
	c_player closestplayer;
	int besttarget;
}

void superglide()
{
	while (true)
	{
		if (settings::engine)
		{
			if (settings::superglide)
			{
				c_player localplayer = read<c_player>(virtualaddy + OFF_LOCAL_PLAYER);
				if (GetAsyncKeyState(settings::superbind) & 0x8000) // Key is held
				{
					if (GetAsyncKeyState(VK_SPACE) & 0x8000)
					{
						float HangOnWall1 = 0.05;
						float HangOnWall2 = 0.07;
						float TraversalProgress1 = 0.90f;
						float HangOnWall3 = 0.05f;
						float HangOnWall4 = 0.75f;
						float StartJumpTime1 = 0.007;



						static float startjumpTime = 0;
						static bool startSg = false;
						static float traversalProgressTmp = 0.0;

						float worldtime = read<float>(localplayer.get_address() + OFF_TIME_BASE); // Current time
						float traversalStartTime = read<float>(localplayer.get_address() + OFF_TRAVERSAL_START_TIME); // Time to start wall climbing
						float traversalProgress = read<float>(localplayer.get_address() + OFF_TRAVERSAL_PROGRESS); // Wall climbing, if > 0.87 it is almost over.
						float HangOnWall = -(traversalStartTime - worldtime);

						if (HangOnWall > HangOnWall1 && HangOnWall < HangOnWall2) {
							write<int>(virtualaddy + OFF_IN_JUMP + 0x8, 4);
						}
						if (traversalProgress > TraversalProgress1 && !startSg && HangOnWall > HangOnWall3 && HangOnWall < HangOnWall4) {
							// start SG
							startjumpTime = worldtime;
							startSg = true;
						}
						if (startSg) {
							write<int>(virtualaddy + OFF_IN_JUMP + 0x8, 5);
							while (read<float>(localplayer.get_address() + OFF_TIME_BASE) - startjumpTime < StartJumpTime1)
								; // why is this here...
							{
								write<int>(virtualaddy + OFF_IN_DUCK + 0x8, 6);
								std::this_thread::sleep_for(std::chrono::milliseconds(50));
								write<int>(virtualaddy + OFF_IN_JUMP + 0x8, 4);
								std::this_thread::sleep_for(std::chrono::milliseconds(600));
							}
							startSg = false;
						}
					}
				}
			}
		}
	}
}

void fovchanger()
{
	while (true)
	{
		if (settings::engine)
		{
			if (settings::fovchanger)
			{
				c_player localplayer = read<c_player>(virtualaddy + OFF_LOCAL_PLAYER);
				if (settings::fovchanger)
				{
					float fov_new = settings::desirefov / 100.0f;
					write<float>(localplayer.get_address() + 0x4810, fov_new);
				}
			}
		}
	}
}

void tapstrafe() {
	while (true)
	{
		if (settings::engine)
		{
			if (settings::tapstrafe)
			{
				bool ts_start = true;
				bool longclimb = false;
				c_player localplayer = read<c_player>(virtualaddy + OFF_LOCAL_PLAYER);
				float wallrun_start = read<float>(localplayer.get_address() + 0x36fc);
				float wallrun_clear = read<float>(localplayer.get_address() + 0x3700);
				float world_time = read<float>(localplayer.get_address() + OFF_TIME_BASE);

				if (wallrun_start > wallrun_clear) {
					float climbTime = world_time - wallrun_start;
					if (climbTime > 0.8) {
						longclimb = true;
						ts_start = false;
					}
					else {
						ts_start = true;
					}
				}
				if (ts_start) {
					if (longclimb) {
						if (world_time > wallrun_clear + 0.1)
							longclimb = false;
					}

					int flags = read<int>(virtualaddy + 0x00c8);
					int backward_state = read<int>(virtualaddy + OFF_IN_BACKWARD);
					int forward_state = read<int>(virtualaddy + OFF_IN_FORWARD);
					int force_forward = read<int>(virtualaddy + OFF_IN_FORWARD + 0x8);
					int skydrive_state = read<int>(localplayer.get_address() + 0x4854);
					int duck_state = read<int>(localplayer.get_address() + 0x2ab8);

					if (((flags & 0x1) == 0) && !(skydrive_state > 0) && !longclimb &&
						!(backward_state > 0)) {
						if (((duck_state > 0) && (forward_state == 33))) { // Previously 33
							if (force_forward == 0) {
								write<int>(virtualaddy + OFF_IN_FORWARD + 0x8, 1);
							}
							else {
								write<int>(virtualaddy + OFF_IN_FORWARD + 0x8, 0);
							}
						}
					}
					else if ((flags & 0x1) != 0) {
						if (forward_state == 0) {
							write<int>(virtualaddy + OFF_IN_FORWARD + 0x8, 0);
						}
						else if (forward_state == 33) {
							write<int>(virtualaddy + OFF_IN_FORWARD + 0x8, 1);
						}
					}

				}

			}
		}
	}
}

void loop()
{
	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	// Screen center
	float screenWidth = io.DisplaySize.x;
	float screenHeight = io.DisplaySize.y;

	// Curved rectangle size and position
	ImVec2 rectSize(290, 25);
	ImVec2 rectPos((screenWidth - rectSize.x) / 2, 20);
	float cornerRadius = 0.f;

	// Background box
	draw->AddRectFilled(rectPos, ImVec2(rectPos.x + rectSize.x, rectPos.y + rectSize.y), IM_COL32(12, 12, 12, 155), cornerRadius);


	if (settings::ragefov)
		settings::fov = 100000.f;


	// Load your texture elsewhere; here assume it's loaded as:
	// ImTextureID catImage;
	// float imageWidth = 60, imageHeight = 60;
	ImVec2 imageSize(20, 20);
	ImVec2 imagePos(rectPos.x + 10, rectPos.y + (rectSize.y - imageSize.y) / 2);
	draw->AddImage(sillycar, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

	// First vertical barrier
	float barrierX1 = imagePos.x + imageSize.x + 10;
	draw->AddLine(ImVec2(barrierX1, rectPos.y + 10), ImVec2(barrierX1, rectPos.y + rectSize.y - 10), IM_COL32(100, 100, 100, 255), 1.0f);

	// Cheat name "energy"
	ImVec2 textPos1(barrierX1 + 10, rectPos.y + (rectSize.y / 2) - ImGui::CalcTextSize("eax by cen").y / 2);
	draw->AddText(textPos1, IM_COL32_WHITE, "eax by cen");

	// Second vertical barrier
	float barrierX2 = textPos1.x + ImGui::CalcTextSize("eax by cen").x + 10;
	draw->AddLine(ImVec2(barrierX2, rectPos.y + 10), ImVec2(barrierX2, rectPos.y + rectSize.y - 10), IM_COL32(100, 100, 100, 255), 1.0f);

	// Current time
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);

	// Use ctime_s to format time safely
	char timeBuffer[26]; // ctime_s requires a 26-byte buffer
	errno_t err = ctime_s(timeBuffer, sizeof(timeBuffer), &now_c);
	std::string timeStr = (err == 0) ? std::string(timeBuffer) : "Invalid Time";

	// Remove newline from end if present
	if (!timeStr.empty() && timeStr.back() == '\n') {
		timeStr.pop_back();
	}
	ImVec2 textPos2(barrierX2 + 10, rectPos.y + (rectSize.y / 2) - ImGui::CalcTextSize(timeStr.c_str()).y / 2);
	draw->AddText(textPos2, IM_COL32_WHITE, timeStr.c_str());

	if (settings::crosshair)
	{
		ImVec2 center(screenWidth / 2, screenHeight / 2);
		float crosshairSize = 10.0f;
		draw->AddLine(ImVec2(center.x - crosshairSize, center.y), ImVec2(center.x + crosshairSize, center.y), IM_COL32(255, 0, 0, 255), 1.0f);
		draw->AddLine(ImVec2(center.x, center.y - crosshairSize), ImVec2(center.x, center.y + crosshairSize), IM_COL32(255, 0, 0, 255), 1.0f);
	}

	if (settings::showfov) ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), settings::fov, ImColor(255, 0, 0), 100, 1.0f);

	SetWindowDisplayAffinity(Overlay.Hwnd, settings::streamproof ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);

	c_player local_player(read<DWORD_PTR>(virtualaddy + OFF_LOCAL_PLAYER));
	if (!local_player.valid()) {
		return;
	}
	int best_target_index = 0;


	if (settings::antiaim)
	{
		c_player local_player(read<DWORD_PTR>(virtualaddy + OFF_LOCAL_PLAYER));
		auto health = local_player.i_health();

		uint64_t vmhandle = read<uint64_t>(local_player.get_address() + OFF_VIEWMODEL) & 0xFFFF;
		if (!vmhandle) return;

		auto view_model_ptr = read<uint64_t>(virtualaddy + OFF_ENTITY_LIST + (vmhandle << 5));
		if (!view_model_ptr) return;
 
		c_vec3 angles = calculate_antiaim_angles(settings::Pitch, settings::Yaw);
		write<c_vec3>(local_player.get_address() + 0x194, (rand() % 100, rand() % -250, rand() % 500)); // m_angNetworkAngles server / client
		write<c_vec3>(view_model_ptr + 0x194, (rand() % 100, rand() % -250, rand() % 500)); // m_angNetworkAngles View Module
	}

	// 0x02562050
	if (settings::fakefire)
	write<int>(local_player.get_address() + 0x1cc4, 1132396544);

	if (settings::engine)
	{
		if (settings::localmodelchams)
		{

			uint64_t HighlightSettings = read<uint64_t>(virtualaddy + 0x68E5030);
			if (!HighlightSettings) return;

			WP_HighlightFunctionBits highlightFunctionBits = {
				{
					1,   // InsideFunction
					0, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
					0,  // OutlineRadius: size * 255 / 8
					0   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
				}
			};

			WP_HighlightParameter glowColorRGB = { { settings::rgb[0],
											 settings::rgb[1],
											settings::rgb[2],
											 155} };

			static const int contextId = 0;
			int settingIndex = 0;

			write<int>(local_player.get_address() + 0x26C, 1);
			write<int>(local_player.get_address() + 0x278, 2);
			write<int>(local_player.get_address() + 0x29C + contextId, settingIndex);

			write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
			write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
		}

		if (settings::fullbright)
		{
			//0x01aced10
			write <DWORD64>(virtualaddy + 0x01aced10 + 0x64, 1); // r_fullbright
		}
		else
		{
			write <DWORD64>(virtualaddy + 0x01aced10 + 0x64, 0); // r_fullbright
		}

	
		if (settings::third_person)
		{
			static bool wasKeyPressed = false;
			static bool isThirdPersonToggled = false;

			SHORT keyState = GetAsyncKeyState(settings::keybind3rd);

			// If key is currently pressed
			if (keyState & 0x8000)
			{
				if (!wasKeyPressed)
				{
					// Toggle third person state
					isThirdPersonToggled = !isThirdPersonToggled;
					wasKeyPressed = true;

					// Write to memory
					write<int>(virtualaddy + 0x24E7780 + 0x64, isThirdPersonToggled ? 1 : 0);

					wasKeyPressed = true;
				}
			}
			else
			{
				// Reset when key is released to allow next toggle
				wasKeyPressed = false;
			}
		}
		else if (!settings::third_person)
		{
			write<DWORD64>(virtualaddy + 0x024e7780 + 0x64, 0);
		}

		if (settings::noviewmodel)
		write<DWORD64>(virtualaddy + 0x026a5a80 + 0x64, 0); // r_drawviewmodel
		else
		{
			write<DWORD64>(virtualaddy + 0x026a5a80 + 0x64, 1);
		}

		if (settings::nosky)
		{
			write<DWORD64>(virtualaddy + 0x026a5b20 + 0x64, 0); // r_drawviewmodel
		}
		else
		{
			write<DWORD64>(virtualaddy + 0x026a5b20 + 0x64, 1); // r_drawviewmodel
		}


		if (settings::viewmodelglow)
		{
			int64_t actWeaponID = read<uint64_t>(local_player.get_address() + OFF_VIEWMODEL) & 0xFFFF;
			uint64_t currentWeapon = read<uint64_t>(virtualaddy + OFF_ENTITY_LIST + (actWeaponID << 5));

			uint64_t HighlightSettings = read<uint64_t>(virtualaddy + 0x68E5030);
			if (!HighlightSettings) return;


			static const int contextId = 0;
			int settingIndex = 0;

			WP_HighlightFunctionBits highlightFunctionBits = {
				{
					1,   // InsideFunction
					0, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
					0,  // OutlineRadius: size * 255 / 8
					0   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
				}
			};

			WP_HighlightParameter glowColorRGB = { { settings::rgb[0],
											 settings::rgb[1],
											settings::rgb[2],
											 155} };

			if (settings::viewmodelglow)
			{
				write<int>(currentWeapon + 0x26C, 1);
				write<int>(currentWeapon + 0x278, 2);
				write<int>(currentWeapon + 0x29C + contextId, settingIndex);

				write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
				write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
			}
			else
			{
				//write<int>(currentWeapon + 0x26C, 0);
				write<int>(currentWeapon + 0x278, 0);
				write<int>(currentWeapon + 0x29C + contextId, 0);

				write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
				write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
			}
		}

		if (settings::handglow)
		{
			int64_t actWeaponID = read<uint64_t>(local_player.get_address() + 0x2dec) & 0xFFFF;
			uint64_t currentWeapon = read<uint64_t>(virtualaddy + OFF_ENTITY_LIST + (actWeaponID << 5));
			uint64_t HighlightSettings = read<uint64_t>(virtualaddy + 0x68E5030);
			if (!HighlightSettings) return;

			WP_HighlightFunctionBits highlightFunctionBits = {
				{
					1,   // InsideFunction
					0, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
					0,  // OutlineRadius: size * 255 / 8
					0   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
				}
			};

			WP_HighlightParameter glowColorRGB = { { settings::rgb[0],
											 settings::rgb[1],
											settings::rgb[2],
											 155} };

			static const int contextId = 0;
			int settingIndex = 0;


			if (settings::handglow)
			{
				write<int>(currentWeapon + 0x26C, 1);
				write<int>(currentWeapon + 0x278, 2);
				write<int>(currentWeapon + 0x29C + contextId, settingIndex);

				write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
				write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
			}
			else
			{
			//	write<int>(currentWeapon + 0x26C, 0);
				write<int>(currentWeapon + 0x278, 0);
				write<int>(currentWeapon + 0x29C + contextId, 0);

				write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
				write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
			}
		}
	}

	if (settings::skinchanger)
	{
		DWORD64 LastWeapon = read<DWORD64>(local_player.get_address() + 0x1994) & 0xFFFF; //m_latestPrimaryWeapons
		DWORD64 Weapon = read<DWORD64>(virtualaddy + 0x619baa8 + (LastWeapon << 5)); //cl_entitylist
		write<int>(Weapon + 0xd40, settings::skinid);

	}

	if (settings::knifechanger)
	{
		if (!loopae)
			std::thread(heirloom).detach();
		loopae = true;
	}

	float closest_distance = FLT_MAX;
	c_player closest_player;
	for (auto x = 0; x < player_list.size(); ++x) {
		c_player& player(player_list[x]);
		if (settings::teamcheck)
			if (player.i_team() == local_player.i_team()) continue;

		auto distance = player.distance() / 100.0f;
		auto name = player.s_name();
		auto health = player.i_health();
		auto shield = player.i_shield();


		auto head = player.bone_position(8);
		auto feet = player.bone_position(0);
		auto origin = player.vec_origin();
		origin = world_to_screen(origin);
		head = world_to_screen(head);
		feet = world_to_screen(feet);

		float box_height = abs(head.y - origin.y);
		float box_width = box_height * 0.50f;
		float box_x = head.x - (box_width / 2);
		float box_y = head.y;

		if (settings::headdot)
		{
			draw_filled_circle(head.x, head.y, ImColor(255, 255, 255), 1.5f);
		}

		if (settings::box)
		{
			draw_cornered_box(head.x - (box_width / 2), head.y, box_width, box_height, ImColor(settings::boxcolor[0], settings::boxcolor[1], settings::boxcolor[2]), 1.f);
		}

		if (settings::health)
		{
			
			int delta = health * box_height / 100;

			draw_rectangle_filled(box_x - 4, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(20, 20, 20, 255));
			draw_rectangle_filled(box_x - 4, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(0, 255, 0, 255));
		}

		if (settings::sheild)
		{
			int delta = shield * box_height / 100;

			float shield_bar_x = settings::health ? box_x - 6 : box_x - 2;

			draw_rectangle_filled(shield_bar_x, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(20, 20, 20, 255));
			draw_rectangle_filled(shield_bar_x, box_y + (box_height - delta) + 1, 2, delta - 1, IM_COL32(40, 40, 255, 255));
		}

		if (settings::name) {
			ImVec2 text_size = ImGui::CalcTextSize(name.c_str());
			float text_x = box_x + (box_width / 2) - (text_size.x / 2);
			float text_y = box_y - text_size.y - 2;

			draw_text(text_x, text_y, ImColor(255, 255, 255), name.c_str());
		}

	

		if (settings::engine)
		{
			if (settings::glow)
			{
				string vischeck;

				if (!player.visible(x))
					vischeck = "not visible";
				else
					vischeck = "visible";


				ImVec2 text_size = ImGui::CalcTextSize(vischeck.c_str());
				float text_x = box_x + (box_width / 2) - (text_size.x / 2);
				float text_y = box_y - text_size.y - 6;

				draw_text(text_x, text_y, ImColor(255, 255, 255), vischeck.c_str());


				uint64_t HighlightSettings = read<uint64_t>(virtualaddy + 0x68E5030);
				if (!HighlightSettings) return;

				WP_HighlightFunctionBits highlightFunctionBits = {
					{
						1,   // InsideFunction
						0, // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
						0,  // OutlineRadius: size * 255 / 8
						0   // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
					}
				};

				WP_HighlightParameter glowColorRGB = { { settings::rgb[0],
												 settings::rgb[1],
												settings::rgb[2],
												 155} };

				static const int contextId = 0;
				int settingIndex = 0;

				write<int>(player.get_address() + 0x26C, 1);
				write<int>(player.get_address() + 0x278, 2);
				write<int>(player.get_address() + 0x29C + contextId, settingIndex);

				write<WP_HighlightFunctionBits>(HighlightSettings + 0x34 * settingIndex + 0, highlightFunctionBits);
				write<WP_HighlightParameter>(HighlightSettings + 0x34 * settingIndex + 4, glowColorRGB);
			}
		}

		double dx = head.x - Width / 2;
		double dy = head.y - Height / 2;
		float dist = sqrtf(static_cast<float>(dx * dx + dy * dy));

		if (dist <= settings::fov && dist < closest_distance && dist != 100.f)
		{
			closest_distance = dist;
			closest_player = player;
			best_target_index = x;

			trigger::closestplayer = player;
			trigger::besttarget = best_target_index;
		}
	}

	if (settings::aimbot && !settings::menu)
	{
		if (closest_distance <= settings::fov && closest_player.valid())
		{
			c_vec3 target_pos;

			if (settings::aimpart == settings::HEAD)
			{
				auto head = closest_player.bone_position(8);
				head = world_to_screen(head);
				target_pos = head;
			}
			else if (settings::aimpart == settings::PELVIS)
			{
				auto pelvis = closest_player.bone_position(3);
				pelvis = world_to_screen(pelvis);
				target_pos = pelvis;
			}

			if (GetAsyncKeyState(settings::key) & 0x8000) // Key is held
			{
				if (settings::vischeck && !closest_player.visible(best_target_index))
					return;

				aimbot(target_pos.x, target_pos.y);
			}
		}
	}
}

void TriggerbotLoop()
{
	int screenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int screenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
	while (true)
	{
		if (settings::triggerbot && !settings::menu)
		{
				if (settings::vischeck && !trigger::closestplayer.visible(trigger::besttarget))
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}

				c_vec3 hitboxPos = trigger::closestplayer.bone_position(8);
				c_vec3 screenPos = world_to_screen(hitboxPos);

				float dx = screenPos.x - screenCenterX;
				float dy = screenPos.y - screenCenterY;
				float distance = sqrtf(dx * dx + dy * dy);

				const float triggerRadius = 15.0f;

				if (distance <= triggerRadius)
				{

					if (GetAsyncKeyState(settings::keybind) & 0x8000) // Trigger key held
					{
						if (settings::ms > 0)
							Sleep(settings::ms);

						input::mouse_click();
					}
				}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Small sleep to reduce CPU load
	}
}

void setup()
{
	while (true)
	{
		std::vector<c_player> players;
		c_player local_player(read<DWORD_PTR>(virtualaddy + OFF_LOCAL_PLAYER));
		if (local_player.valid()) {
			for (auto x = 0; x < 100 /*g_pglobalvars.maxClients + 1*/; ++x) {
				auto entity = read<DWORD_PTR>(virtualaddy + OFF_ENTITY_LIST + (x * 0x20));
				if (!entity) continue;
				c_player player(entity);

				if (player != local_player && player.alive()) {
					auto c_signifier = class_signifier(entity);
					if (c_signifier != ("player")) 
						continue;
				}

				players.push_back(player);
			}
		}

		player_list = players;
		players.clear();
		Sleep(10);
	}
}

void render() {

	static float menuAlpha = 0.0f;
	float fadeSpeed = ImGui::GetIO().DeltaTime * 24.0f;
	if (GetAsyncKeyState(VK_INSERT) & 1) settings::menu = !settings::menu;

	menuAlpha = ImLerp(menuAlpha, settings::menu ? 1.0f : 0.0f, fadeSpeed);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	loop();

	ImGui::GetIO().MouseDrawCursor = settings::menu;



	if (menuAlpha > 0.01f) {
		ImGui::SetNextWindowBgAlpha(menuAlpha);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menuAlpha);

		InputHandler();
		drawmenu();

		if (settings::menu) {
			SetWindowLong(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
			UpdateWindow(Overlay.Hwnd);
			SetFocus(Overlay.Hwnd);
		}
		else if (menuAlpha < 0.02f) {
			SetWindowLong(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
			SetFocus(nullptr);
		}

		ImGui::PopStyleVar();
	}

	ImGui::EndFrame();

	DirectX9.pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9.pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9.pDevice->EndScene();
	}

	HRESULT result = DirectX9.pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9.pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9.pDevice->Reset(&DirectX9.pParameters);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}


void render_loop()
{
	static RECT OldRect = { 0 };
	static HWND LastTempHwnd = nullptr;
	static DWORD lastSizeCheck = GetTickCount();
	static const int sizeCheckIntervalMs = 1000;
	static const int targetFPS = 144;
	static const DWORD frameDelay = 1000 / targetFPS;

	ZeroMemory(&DirectX9.Message, sizeof(MSG));

	while (DirectX9.Message.message != WM_QUIT) {
		DWORD frameStart = GetTickCount();

		if (PeekMessage(&DirectX9.Message, Overlay.Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9.Message);
			DispatchMessage(&DirectX9.Message);
		}

		SetWindowPos(
			Overlay.Hwnd,
			HWND_TOPMOST,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW
		);

		DWORD now = GetTickCount();
		if (now - lastSizeCheck > sizeCheckIntervalMs) {
			lastSizeCheck = now;

			RECT TempRect = { 0 };
			POINT TempPoint = { 0 };
			GetClientRect(Process.Hwnd, &TempRect);
			ClientToScreen(Process.Hwnd, &TempPoint);

			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;

			if (memcmp(&TempRect, &OldRect, sizeof(RECT)) != 0) {
				OldRect = TempRect;
				Width = TempRect.right;
				Height = TempRect.bottom;

				DirectX9.pParameters.BackBufferWidth = Width;
				DirectX9.pParameters.BackBufferHeight = Height;

				SetWindowPos(
					Overlay.Hwnd,
					HWND_TOPMOST,
					TempPoint.x, TempPoint.y,
					Width, Height,
					SWP_NOACTIVATE | SWP_SHOWWINDOW
				);

				if (DirectX9.pDevice) {
					HRESULT resetResult = DirectX9.pDevice->Reset(&DirectX9.pParameters);
					if (FAILED(resetResult)) {
					}
				}
			}
		}

		render();
		Sleep(0);
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (DirectX9.pDevice) {
		DirectX9.pDevice->EndScene();
		DirectX9.pDevice->Release();
		DirectX9.pDevice = nullptr;
	}

	if (DirectX9.IDirect3D9) {
		DirectX9.IDirect3D9->Release();
		DirectX9.IDirect3D9 = nullptr;
	}

	DestroyWindow(Overlay.Hwnd);
	UnregisterClass(Overlay.WindowClass.lpszClassName, Overlay.WindowClass.hInstance);
}