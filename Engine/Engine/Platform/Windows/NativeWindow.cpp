#include "Engine/Platform/Windows/NativeWindow.h"
#include "Engine/Platform/Windows/UnicodeHelper.h"
#include <ShellScalingApi.h>
#include "Engine/Application/Engine.h"
#include "Engine/System/Thread/JobSystem.h"

namespace Engine::PlatformSpecific::Windows {
	typename NativeWindowManager::_Initializer NativeWindowManager::_initializer{};

	NativeWindowManager::_Initializer::_Initializer() {
		// Make console support UTF-8
		SetConsoleOutputCP(65001);

		SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

		// Register basic window class
		WNDCLASSW wc = {};
		wc.lpszClassName = NativeWindow::GlobalWindowClassName;
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
		wc.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
		wc.lpszMenuName = NULL;
		wc.hInstance = NULL;
		wc.lpfnWndProc = NativeWindow::WndProc;

		bool succeeded = RegisterClassW(&wc);
		FATAL_ASSERT(succeeded, u8"RegisterClassW failed to register window class!");
	}

	NativeWindowManager::_Initializer::~_Initializer() {
		UnregisterClassW(NativeWindow::GlobalWindowClassName, NULL);
	}

	void NativeWindowManager::Update() {
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

	NativeWindow* NativeWindow::GetFromHWnd(HWND hWnd) {
		if (!IsWindow(hWnd)) {
			return nullptr;
		}

		LONG_PTR p = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		if (p == NULL) {
			return nullptr;
		}

		NativeWindow* wp = (NativeWindow*)p;

		return wp;
	}




	struct _NWWInit {
		LONG_PTR userDataPtr;
		HWND result;
	};
	struct _NWWDestroy {
		HWND hWnd;
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


	bool NativeWindow::Initialize() {
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWInit>();

			HWND w = CreateWindowExW(DefaultWindowExStyle,GlobalWindowClassName, L"", DefaultWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
			if (!IsWindow(w)) {
				data->result = NULL;
				return;
			}
			// Set user data to let hwnd trace back to NativeWindow.
			SetWindowLongPtrW(w, GWLP_USERDATA, data->userDataPtr);

			data->result = w;
		};
		
		// Job data
		_NWWInit data = {};
		data.userDataPtr = (LONG_PTR)this;
		// Start job
		auto js = ENGINEINST->GetJobSystem();
		auto job=js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
		// Wait for the result
		js->WaitJob(job);

		HWND w = job->GetDataAs<_NWWInit>()->result;
		ERR_ASSERT(IsWindow(w), u8"CreateWindowW failed to create a window!", return false);
		hWnd = w;
		return true;
	}

	NativeWindow::~NativeWindow() {
		if (!IsWindow(hWnd)) {
			return;
		}

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWDestroy>();
			DestroyWindow(data->hWnd);
		};
		_NWWDestroy data = {};
		data.hWnd = hWnd;

		auto js = ENGINEINST->GetJobSystem();
		js->AddJob(func, &data, sizeof(data), Job::Preference::Window);
	}

	LRESULT CALLBACK NativeWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
			{
				NativeWindow* nw = NativeWindow::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					nw->GetManager()->Destroy(nw->GetId());
				} else {
					ERR_MSG(u8"User data in hWnd is not a NativeWindow ptr! This shouldn't happen!");
					DestroyWindow(hWnd);
				}
			}
				return 0;

			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;

			case WM_KEYDOWN:
			{
				NativeWindow* nw = NativeWindow::GetFromHWnd(hWnd);
				if (nw != nullptr) {
					Variant key = wParam;
					const Variant* args[1] = { &key };
					nw->EmitSignal(STRL("KeyDown"), (const Variant**)args, 1);
				}
				break;
			}
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	bool NativeWindow::IsValid() const {
		return IsWindow(hWnd);
	}
	
	String NativeWindow::GetTitle() const {
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
	bool NativeWindow::SetTitle(const String& title) {
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

	Vector2 NativeWindow::GetPosition() const {
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
	bool NativeWindow::SetPosition(const Vector2& position) {
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
	
	Vector2 NativeWindow::GetSize() const {
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
	bool NativeWindow::SetSize(const Vector2& size) {
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

	bool NativeWindow::IsVisible() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_VISIBLE);
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
	bool NativeWindow::SetVisible(bool visible) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);
		
		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_VISIBLE, data->enabled);
			UpdateWindow(data->hWnd);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = visible;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}
	
	bool NativeWindow::IsMinimized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_MINIMIZE);
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
	bool NativeWindow::SetMinimized(bool minimized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_MINIMIZE, data->enabled);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = minimized;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}
	bool NativeWindow::IsMaximized() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_MAXIMIZE);
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
	bool NativeWindow::SetMaximized(bool maximized) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_MAXIMIZE, data->enabled);
			data->result = succeeded;
		};
		_NWWSetStyleFlag data = {};
		data.hWnd = hWnd;
		data.enabled = maximized;

		auto js = ENGINEINST->GetJobSystem();
		auto job = js->AddJob(func, &data, sizeof(data), Job::Preference::Window);

		js->WaitJob(job);

		bool succeeded = job->GetDataAs<_NWWSetStyleFlag>()->result;
		return succeeded;
	}
	bool NativeWindow::HasMinimizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_MINIMIZEBOX);
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
	bool NativeWindow::SetMinimizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_MINIMIZEBOX, data->enabled);
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
	bool NativeWindow::HasMaximizeButton() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_MAXIMIZEBOX);
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
	bool NativeWindow::SetMaximizeButton(bool enabled) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_MAXIMIZEBOX, data->enabled);
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

	bool NativeWindow::HasBorder() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_BORDER);
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
	bool NativeWindow::SetBorder(bool enabled) {
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

			bool succeeded = NativeWindow::SetStyleFlag(hWnd, WS_BORDER | WS_CAPTION, data->enabled);
			
			bool adjusted = AdjustWindowRectEx(&rect, NativeWindow::GetStyle(hWnd), FALSE, NativeWindow::GetExStyle(hWnd));
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

	bool NativeWindow::IsResizable() const {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWHasStyleFlag>();
			bool result = NativeWindow::HasStyleFlag(data->hWnd, WS_SIZEBOX);
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
	bool NativeWindow::SetResizable(bool resizable) {
		ERR_ASSERT(IsValid(), u8"The window is not valid!", return false);

		auto func = [](Job* job) {
			auto data = job->GetDataAs<_NWWSetStyleFlag>();
			bool succeeded = NativeWindow::SetStyleFlag(data->hWnd, WS_SIZEBOX, data->enabled);
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

	HWND NativeWindow::GetHWnd() const {
		return hWnd;
	}

	DWORD NativeWindow::GetStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_STYLE);
	}
	DWORD NativeWindow::GetStyle() const {
		return GetStyle(hWnd);
	}

	DWORD NativeWindow::GetExStyle(HWND hWnd) {
		return GetWindowLongW(hWnd, GWL_EXSTYLE);
	}
	DWORD NativeWindow::GetExStyle() const {
		return GetExStyle(hWnd);
	}

	bool NativeWindow::HasStyleFlag(HWND hWnd,DWORD style) {
		return GetStyle(hWnd) & style;
	}
	bool NativeWindow::HasStyleFlag(DWORD style) const {
		return HasStyleFlag(hWnd, style);
	}

	bool NativeWindow::SetStyleFlag(HWND hWnd, DWORD style, bool enabled) {
		DWORD s = GetStyle(hWnd);
		if (enabled) {
			s |= style;
		} else {
			s &= ~style;
		}
		SetWindowLongW(hWnd, GWL_STYLE, s);
		return true;
	}
	bool NativeWindow::SetStyleFlag(DWORD style, bool enabled) {
		return SetStyleFlag(hWnd, style, enabled);
	}
}
