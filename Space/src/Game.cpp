#include <Windows.h>
#include <iostream>
#include "GL/glew.h"
#include "GL/wglew.h"
#include "IwMath/matrix4.h"
#include "IwInput/input_manager.h"
#include "graphics/mesh.h"
#include "graphics/shader_program.h"

LRESULT CALLBACK win_proc(HWND h_wnd, UINT msg, WPARAM w_parm, LPARAM l_param);

ATOM register_class(HINSTANCE h_instance) {
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = win_proc;
	wcex.hInstance = h_instance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName = "Core";

	return RegisterClassEx(&wcex);
}

bool running = true;

LRESULT CALLBACK win_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(w_param);
		break;
	case WM_CLOSE:
		running = false;
		break;
	default:
		return DefWindowProc(h_wnd, msg, w_param, l_param);
	}
}

int CALLBACK WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	LPTSTR window = MAKEINTATOM(register_class(h_instance));

	HWND fake_h_wnd = CreateWindow(
		"Core", "Fake Window",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		1, 1,
		NULL, NULL,
		h_instance, NULL);

	HDC fake_dc = GetDC(fake_h_wnd);

	PIXELFORMATDESCRIPTOR fake_pfd;
	ZeroMemory(&fake_pfd, sizeof(fake_pfd));
	fake_pfd.nSize = sizeof(fake_pfd);
	fake_pfd.nVersion = 1;
	fake_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fake_pfd.iPixelType = PFD_TYPE_RGBA;
	fake_pfd.cColorBits = 32;
	fake_pfd.cAlphaBits = 8;
	fake_pfd.cDepthBits = 24;

	int fake_pfd_id = ChoosePixelFormat(fake_dc, &fake_pfd);
	if (fake_pfd_id == 0) {
		std::cout << "ChoosePixelFormat() failed!";
		return 1;
	}

	if (!SetPixelFormat(fake_dc, fake_pfd_id, &fake_pfd)) {
		std::cout << "SetPixelFormat() failed!";
		return 1;
	}

	HGLRC fake_rc = wglCreateContext(fake_dc);
	if (fake_rc == 0) {
		std::cout << "wglCreateContext() failed!";
		return 1;
	}

	if (!wglMakeCurrent(fake_dc, fake_rc)) {
		std::cout << "wglMakeCurrent() failed!";
		return 1;
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "glewInit() failed!";
		return 1;
	}

	HWND h_wnd = CreateWindow(
		"Core", "Space",
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		100, 100,
		1280, 720,
		NULL, NULL,
		h_instance, NULL);

	HDC dc = GetDC(h_wnd);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pfid;
	UINT format_count;
	bool status = wglChoosePixelFormatARB(dc, pixelAttribs, NULL, 1, &pfid, &format_count);

	if (status == false || format_count == 0) {
		std::cout << "wglChoosePixelFormatARB() failed!";
		return 1;
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(dc, pfid, sizeof(pfd), &pfd);
	SetPixelFormat(dc, pfid, &pfd);

	const int major = 4, minor = 5;
	int context_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	HGLRC rc = wglCreateContextAttribsARB(dc, 0, context_attribs);
	if (rc == NULL) {
		std::cout << "wglCreateContextAttribsARB() failed!";
		return 1;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fake_rc);
	ReleaseDC(fake_h_wnd, fake_dc);
	DestroyWindow(fake_h_wnd);
	if (!wglMakeCurrent(dc, rc)) {
		std::cout << "wglMakeCurrent() failed.";
		return 1;
	}

	ShowWindow(h_wnd, n_cmd_show);

	graphics::shader_program shader = graphics::shader_program("res/test.shader");

	enum space_input {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	iwinput::input_manager inputManager = iwinput::input_manager();
	unsigned int deviceId = inputManager.create_device(iwinput::KEYBOARD);

	iwinput::input_context* context = inputManager.create_context("Spaceship");
	context->bind_input(FORWARD, iwinput::W, deviceId);
	context->bind_input(BACKWARD, iwinput::S, deviceId);
	context->bind_input(LEFT, iwinput::A, deviceId);
	context->bind_input(RIGHT, iwinput::D, deviceId);
	context->bind_input(UP, iwinput::SPACE, deviceId);
	context->bind_input(DOWN, iwinput::LEFT_SHIFT, deviceId);

	graphics::mesh* mesh = graphics::mesh::create_sphere(1, 3);
	graphics::mesh* mesh1 = graphics::mesh::create_sphere(1, 1);

	float rot = 0;
	iwmath::vector3 pos;
	iwmath::vector3 playerPos;

	iwmath::matrix4 projection = iwmath::matrix4::create_perspective_field_of_view(1.4f, 1280 / 720.0f, .001f, 1000);

	iwmath::matrix4 world = iwmath::matrix4::identity;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use_program();

		inputManager.update();

		MSG msg;
		while (PeekMessage(&msg, h_wnd, 0, 0, PM_REMOVE)) {
			inputManager.queue_event(msg);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (context->get_state(FORWARD))  { playerPos.z += .05f; }
		if (context->get_state(BACKWARD)) { playerPos.z -= .05f; }
		if (context->get_state(LEFT))	  { playerPos.x += .05f; }
		if (context->get_state(RIGHT))	  { playerPos.x -= .05f; }
		if (context->get_state(UP))		  { playerPos.y -= .05f; }
		if (context->get_state(DOWN))	  { playerPos.y += .05f; }

		mesh->draw(pos, iwmath::quaternion::create_from_euler_angles(0, rot, 0));
		mesh1->draw(pos + iwmath::vector3(5, 0, 0), iwmath::quaternion::create_from_euler_angles(rot, 0, 0));
		rot += .001f;

		iwmath::matrix4 view = iwmath::matrix4::create_translation(playerPos);

		glUniformMatrix4fv(0, 1, GL_FALSE, projection.elements);
		glUniformMatrix4fv(4, 1, GL_FALSE, view.elements);
		glUniformMatrix4fv(8, 1, GL_FALSE, world.elements);

		SwapBuffers(dc);
	}

	delete mesh;

	fclose(fp);
	FreeConsole();

	return 0;
}