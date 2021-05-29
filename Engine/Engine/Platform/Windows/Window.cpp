#include "Engine/Platform/Windows/Window.h"
#include "Engine/Platform/Windows/UnicodeHelper.h"
#include <ShellScalingApi.h>
#include "Engine/Application/Engine.h"
#include "Engine/System/Thread/JobSystem.h"
#include "glad/glad.h"

namespace Engine::PlatformSpecific::Windows {
	typename WindowManager::_Initializer WindowManager::_initializer{};

	WindowManager::_Initializer::_Initializer() {
		// Make console support UTF-8
		SetConsoleOutputCP(65001);

		SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

		// Register basic window class
		WNDCLASSW wc = {};
		wc.lpszClassName = Window::GlobalWindowClassName;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
		wc.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
		wc.lpszMenuName = NULL;
		wc.hInstance = NULL;
		wc.lpfnWndProc = Window::WndProc;

		bool succeeded = RegisterClassW(&wc);
		FATAL_ASSERT(succeeded, u8"RegisterClassW failed to register window class!");
	}

	WindowManager::_Initializer::~_Initializer() {
		UnregisterClassW(Window::GlobalWindowClassName, NULL);
	}

	void WindowManager::Update() {
		auto func = [](Job* job) {
			MSG msg = {};
			if (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE)) {
				if (msg.message != WM_QUIT) {
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
			}
		};
		ENGINEINST->GetJobSystem()->AddJob(func, nullptr, 0, Job::Preference::Window);
	}

	Window* Window::GetFromHWnd(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return nullptr;
		}

		LONG_PTR p = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		if (p == NULL) {
			return nullptr;
		}

		Window* wp = (Window*)p;

		return wp;
	}




	struct _NWWInit {
		Window* window;
		bool result;
		HWND rHWnd;
		HGLRC rHGLRC;
	};
	struct _NWWDestroy {
		HWND hWnd;
		HGLRC hGLRC;
	};
	struct _NWWGetTitle {
		HWND hWnd;
		UniquePtr<WCHAR[]>* buffer;
	};
	struct _NWWSetTitle {
		HWND hWnd;
		WCHAR* title;
		bool result;
	};
	struct _NWWGetVector2 {
		HWND hWnd;
		int32 x;
		int32 y;
	};
	struct _NWWSetVector2 {
		HWND hWnd;
		int32 x;
		int32 y;
		bool result;
	};
	struct _NWWHasStyleFlag {
		HWND hWnd;
		bool result;
	};
	struct _NWWSetStyleFlag {
		HWND hWnd;
		bool enabled;
		bool result;
	};


	bool Window::Initialize() {
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWInit>();

			Window* window = data->window;

#pragma region Set up hWnd
			HWND hWnd = CreateWindowExW(DefaultWindowExStyle,GlobalWindowClassName, L"", DefaultWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, DefaultSizeX, DefaultSizeY, NULL, NULL, NULL, NULL);
			if (!IsWindow(hWnd)) {
				data->result = false;
				ERR_MSG(u8"CreateWindowExW failed to create a window.");
				return;
			}
			// Set user data to let hwnd trace back to Window.
			SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)window);
#pragma endregion

			data->result = true;
			data->rHWnd = hWnd;
			//data->rHGLRC = hGLRC;
		};
		
		// Job data
		_NWWInit data = {};
		data.window = this;
		// Start job
		auto js = ENGINEINST->GetJobSystem();
		auto job=js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		// Wait for the result
		js->WaitJob(job);

		auto result = job->GetDataAs<_NWWInit>();
		ERR_ASSERT(result->result, u8"Window creation routine failed!", return false);

		hWnd = result->rHWnd;
		//renderContext = result->rHGLRC;
		renderContext = InitRender(hWnd);

		return true;
	}

	Window::~Window() {
		if (!IsWindow(hWnd)) {
			return;
		}

		CleanupRender();
		UninitRender(hWnd, (HGLRC)renderContext);
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWDestroy>();
			HWND hWnd = data->hWnd;
			//HGLRC hGLRC = data->hGLRC;

			//auto w = Window::GetFromHWnd(hWnd);
			//w->UninitRender(hWnd, hGLRC);
			DestroyWindow(hWnd);
		};
		_NWWDestroy data = {};
		data.hWnd = hWnd;
		//data.hGLRC = (HGLRC)renderContext;

		auto js = ENGINEINST->GetJobSystem();
		js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
	}

#pragma region Craps
	LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
			{
				Window* nw = Window::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					nw->GetManager()->Destroy(nw->GetId());
				} else {
					ERR_MSG(u8"User data in hWnd is not a Window ptr! This shouldn't happen!");
					DestroyWindow(hWnd);
				}
			}
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			case WM_KEYDOWN:
			{
				Window* nw = Window::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					Variant key = wParam;
					const Variant* args[1] = { &key };
					nw->EmitSignal(STRL("KeyDown"), (const Variant**)args, 1);
				}
				break;
			}

			case WM_PAINT:
			case WM_ERASEBKGND:
				return 0;
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	bool Window::IsValid() const {
		return IsWindow(hWnd);
	}
	
	String Window::GetTitle() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return String::GetEmpty());

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWGetTitle>();
			HWND hWnd = data->hWnd;
			int len = GetWindowTextLengthW(hWnd);
			if (len <= 0) {
				return;
			}
			len += 1;

			UniquePtr<WCHAR[]> buffer = UniquePtr<WCHAR[]>::Create(len);
			GetWindowTextW(hWnd, buffer.GetRaw(), len);

			*(data->buffer) = Memory::Move(buffer);
		};

		UniquePtr<WCHAR[]> buffer;

		_NWWGetTitle data = {};
		data.hWnd = hWnd;
		data.buffer = &buffer;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		if (buffer == nullptr) {
			return String::GetEmpty();
		}
		String result;
		bool succeeded = UnicodeHelper::UnicodeToUTF8(buffer.GetRaw(), result);
		ERR_ASSERT(succeeded, u8"Failed to convert Windows wide string to engine string!", return String::GetEmpty());

		return result;
	}
	bool Window::SetTitle(const String& title) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		UniquePtr<WCHAR[]> buffer;
		bool succeeded = UnicodeHelper::UTF8ToUnicode(title, buffer);
		
		ERR_ASSERT(succeeded, u8"Failed to convert engine string to Windows wide string!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetTitle>();
			bool succeeded = SetWindowTextW(data->hWnd, data->title);
			data->result = succeeded;
		};

		_NWWSetTitle data = {};
		data.hWnd = hWnd;
		data.title = buffer.GetRaw();

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		succeeded = job->GetDataAs<_NWWSetTitle>()->result;
		ERR_ASSERT(succeeded, u8"SetWindowTextW failed to set window title!", return false);
		
		return true;
	}

	Vector2 Window::GetPosition() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWGetVector2>();

			POINT pos{};
			ClientToScreen(data->hWnd, &pos);

			data->x = pos.x;
			data->y = pos.y;
		};

		_NWWGetVector2 data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		auto rdata = job->GetDataAs<_NWWGetVector2>();

		return Vector2(rdata->x, rdata->y);
	}
	bool Window::SetPosition(const Vector2& position) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		RECT rect = {};
		rect.left = (int)position.x;
		rect.top = (int)position.y;
		rect.right = 100;
		rect.bottom = 100;
		bool succeeded = AdjustWindowRectEx(&rect, GetStyle(), FALSE,GetExStyle());
		ERR_ASSERT(succeeded, u8"AdjustWindowRect failed to calculate window rect!", return false);

		// Job
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetVector2>();
			bool succeeded = SetWindowPos(data->hWnd, NULL, data->x, data->y, 0, 0, SWP_NOREPOSITION | SWP_NOSIZE);
			data->result = succeeded;
		};

		_NWWSetVector2 data = {};
		data.hWnd = hWnd;
		data.x = rect.left;
		data.y = rect.top;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		succeeded = job->GetDataAs<_NWWSetVector2>()->result;
		ERR_ASSERT(succeeded, u8"SetWindowPos failed to set window rect!", return false);
		return true;
	}
	
	Vector2 Window::GetSize() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return Vector2());

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWGetVector2>();
			
			RECT rect = {};
			GetClientRect(data->hWnd, &rect);

			data->x = rect.right - rect.left;
			data->y = rect.bottom - rect.top;
		};
		
		_NWWGetVector2 data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		auto rdata = job->GetDataAs<_NWWGetVector2>();

		return Vector2(rdata->x, rdata->y);
	}
	bool Window::SetSize(const Vector2& size) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		RECT rect = {};
		rect.left = 0;
		rect.top = 0;
		rect.right = (int)size.x;
		rect.bottom = (int)size.y;
		AdjustWindowRectEx(&rect, GetStyle(), FALSE,GetExStyle());

		// Job
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetVector2>();
			bool succeeded = SetWindowPos(data->hWnd, NULL, 0, 0, data->x, data->y, SWP_NOREPOSITION | SWP_NOMOVE);
			glViewport(0, 0, data->x, data->y);
			data->result = succeeded;
		};

		_NWWSetVector2 data = {};
		data.hWnd = hWnd;
		data.x = rect.right - rect.left;
		data.y = rect.bottom - rect.top;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetVector2>()->result;
		ERR_ASSERT(succeeded, u8"SetWindowPos failed to set window rect!", return false);
		return true;
	}

	bool Window::IsVisible() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_VISIBLE);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			ShowWindow(data->hWnd, data->enabled ? SW_SHOW : SW_HIDE);
			//UpdateWindow(data->hWnd);
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = visible;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		return true;
	}
	
	bool Window::IsMinimized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_MINIMIZE);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			ShowWindow(data->hWnd, data->enabled ? SW_MINIMIZE : SW_RESTORE);
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = minimized;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		return true;
	}
	bool Window::IsMaximized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_MAXIMIZE);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			ShowWindow(data->hWnd, data->enabled ? SW_MAXIMIZE : SW_RESTORE);
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = maximized;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		return true;
	}
	bool Window::HasCloseButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = GetMenuState(GetSystemMenu(data->hWnd, false), SC_CLOSE, MF_BYCOMMAND) & MF_ENABLED;
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetCloseButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			DWORD flag = (data->enabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED);
			bool succeeded = EnableMenuItem(GetSystemMenu(data->hWnd, false), SC_CLOSE, MF_BYCOMMAND | flag);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = enabled;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}
	bool Window::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_MINIMIZEBOX);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = Window::SetStyleFlag(data->hWnd, WS_MINIMIZEBOX, data->enabled);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = enabled;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}
	bool Window::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_MAXIMIZEBOX);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = Window::SetStyleFlag(data->hWnd, WS_MAXIMIZEBOX, data->enabled);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = enabled;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}

	bool Window::HasBorder() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_BORDER);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetBorder(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			HWND hWnd = data->hWnd;

			POINT pos{};
			ClientToScreen(hWnd, &pos);
			RECT rect = {};
			GetClientRect(hWnd, &rect);
			rect.right += rect.left + pos.x;
			rect.bottom += rect.top + pos.y;
			rect.left = pos.x;
			rect.top = pos.y;

			bool succeeded = Window::SetStyleFlag(hWnd, WS_BORDER | WS_CAPTION, data->enabled);
			
			bool adjusted = AdjustWindowRectEx(&rect, Window::GetStyle(hWnd), FALSE, Window::GetExStyle(hWnd));
			if (adjusted) {
				SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOREPOSITION);
			}
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = enabled;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}

	bool Window::IsResizable() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = Window::HasStyleFlag(data->hWnd, WS_SIZEBOX);
			data->result = result;
		};
		_NWWHasStyleFlag data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		js->WaitJob(job);

		bool result = job->GetDataAs<_NWWHasStyleFlag>()->result;
		return result;
	}
	bool Window::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = Window::SetStyleFlag(data->hWnd, WS_SIZEBOX, data->enabled);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = resizable;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}

	HWND Window::GetHWnd() const {
		return hWnd;
	}

	DWORD Window::GetStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_STYLE);
	}
	DWORD Window::GetStyle() const {
		return GetStyle(hWnd);
	}

	DWORD Window::GetExStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_EXSTYLE);
	}
	DWORD Window::GetExStyle() const {
		return GetExStyle(hWnd);
	}

	bool Window::HasStyleFlag(HWND hWnd,DWORD style) {
		return GetStyle(hWnd) & style;
	}
	bool Window::HasStyleFlag(DWORD style) const {
		return HasStyleFlag(hWnd, style);
	}

	bool Window::SetStyleFlag(HWND hWnd, DWORD style, bool enabled) {
		DWORD s = GetStyle(hWnd);
		if (enabled) {
			s |= style;
		} else {
			s &= ~style;
		}
		SetWindowLongW(hWnd, GWL_STYLE, s);
		return true;
	}
	bool Window::SetStyleFlag(DWORD style, bool enabled) {
		return SetStyleFlag(hWnd, style, enabled);
	}
#pragma endregion

	struct _NWWRender {
		Window* wnd;
	};

	void Window::OnRender() {
		/*auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWRender>();
			data->wnd->DoRender();
		};
		_NWWRender data = {};
		data.wnd = this;
		auto js = ENGINEINST->GetJobSystem();
		auto job=js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		//js->WaitJob(job);*/
		DoRender();
	}

	HGLRC Window::InitRender(HWND hWnd) {
		auto hDC = GetDC(hWnd);

		// Set pixel format
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,0,0,0,0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,0, 0, 0
		};
		int pixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, pixelFormat, &pfd);

		// Create context
		HGLRC hGLRC = wglCreateContext(hDC);
		bool succeeded = wglMakeCurrent(hDC, hGLRC);
		FATAL_ASSERT(succeeded, u8"wglMakeCurrent failed!");


		// Init glad
		succeeded = gladLoadGL();
		FATAL_ASSERT(succeeded, u8"Failed to initialize GLAD!");

		glViewport(0, 0, DefaultSizeX, DefaultSizeY);

		INFO_MSG(String::Format(STRL("OpenGL Device: {0}"), glGetString(GL_RENDERER)).GetRawArray());

		return hGLRC;
	}
	void Window::PrepareRender() {
		INFO_MSG(u8"Preparing render.");
		EmitSignal(STRL("PrepareRender"), nullptr, 0);
	}
	void Window::DoRender() {
		if (!prepared) {
			PrepareRender();
			prepared = true;
		}
		EmitSignal(STRL("Render"), nullptr, 0);
		SwapBuffers(GetDC(hWnd));
	}
	void Window::CleanupRender() {
		INFO_MSG(u8"Cleaning up render.");
		EmitSignal(STRL("CleanupRender"), nullptr, 0);
	}
	void Window::UninitRender(HWND hWnd,HGLRC hGLRC) {
		wglMakeCurrent(GetDC(hWnd), NULL);
		wglDeleteContext(hGLRC);
	}
}
