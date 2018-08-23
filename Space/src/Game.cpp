#include <Windows.h>
#include <iostream>
#include "GL/glew.h"
#include "GL/wglew.h"
#include "IwMath/matrix4.h"
#include "IwMath/matrix3.h"
#include "IwMath/vector2.h"
#include "IwInput/input_manager.h"
#include "graphics/mesh.h"
#include "graphics/shader_program.h"

//Test
#include "ecs/ecs_manager.h"

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

	return 0;
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
	ShowCursor(0);

	graphics::shader_program shader = graphics::shader_program("res/test.shader");

	enum space_input {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		X_AXIS,
		Y_AXIS
	};

	iwinput::input_manager inputManager = iwinput::input_manager();
	inputManager.set_window_info(h_wnd);
	iwinput::keyboard& k = inputManager.create_device<iwinput::keyboard>();
	iwinput::mouse& m = inputManager.create_device<iwinput::mouse>();

	inputManager.recenter_cursor_on_update(false);

	iwinput::input_context* context = inputManager.create_context("Spaceship");
	context->bind_input(FORWARD, iwinput::W, k.id());
	context->bind_input(BACKWARD, iwinput::S, k.id());
	context->bind_input(LEFT, iwinput::A, k.id());
	context->bind_input(RIGHT, iwinput::D, k.id());
	context->bind_input(UP, iwinput::SPACE, k.id());
	context->bind_input(DOWN, iwinput::LEFT_SHIFT, k.id());

	context->bind_input(X_AXIS, iwinput::X_SPEED, m.id());
	context->bind_input(Y_AXIS, iwinput::Y_SPEED, m.id());

	graphics::mesh* mesh = graphics::mesh::create_sphere(1, 3);

	float rot = 0;
	iwmath::vector3 pos;
	iwmath::vector3 playerPos;
	iwmath::vector2 playerRot;

	iwmath::matrix4 projection = iwmath::matrix4::create_perspective_field_of_view(1.4f, 1280 / 720.0f, .001f, 1000);

	iwmath::matrix4 world = iwmath::matrix4::identity;
	
	struct transform {
		float x, y, z;

		transform() {}

		transform(float x, float y, float z)
			: x(x), y(y), z(z) {}
	};

	class transform_system : public iwecs::component_system<transform> {
		void process(component_pack& pack) {

		}
	};

	iwecs::icomponent_system* s = new transform_system();

	iwecs::ecs_manager em = iwecs::ecs_manager();
	em.add_component<transform>(0, 1, 2, 3);
	em.remove_component<transform>(0);
	em.add_component<transform>(0, 4, 5, 6);

	em.add_system<transform_system, transform>();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (running) {
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use_program();

		inputManager.update();

		MSG msg;
		while (PeekMessage(&msg, h_wnd, 0, 0, PM_REMOVE)) {
			inputManager.handle_event(msg);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		iwmath::vector3 moveDelta;
		if (context->get_state(FORWARD)) { moveDelta.z += .05f; }
		if (context->get_state(BACKWARD)) { moveDelta.z -= .05f; }
		if (context->get_state(LEFT)) { moveDelta.x -= .05f; }
		if (context->get_state(RIGHT)) { moveDelta.x += .05f; }
		if (context->get_state(UP)) { moveDelta.y += .05f; }
		if (context->get_state(DOWN)) { moveDelta.y -= .05f; }

		playerPos += moveDelta;

		iwmath::vector2 rotDelta;
		rotDelta.x = context->get_state(X_AXIS);
		rotDelta.y = context->get_state(Y_AXIS);

		rotDelta.x *= 16 / 2.0f;
		rotDelta.y *= 9 / 2.0f;

		playerRot.x = fmod(playerRot.x - rotDelta.x, IW_PI * 2);
		playerRot.y = fmod(playerRot.y + rotDelta.y, IW_PI * 2);

		iwmath::vector3 forward = iwmath::vector3(
			sin(playerRot.x) * cos(playerRot.y),
			sin(playerRot.y),
			cos(playerRot.x) * cos(playerRot.y));

		iwmath::matrix4 view = iwmath::matrix4::create_look_at(playerPos, playerPos - forward, iwmath::vector3::unit_y);

		mesh->draw(pos, iwmath::quaternion::create_from_euler_angles(0, rot, 0));
		rot += .001f;

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