#include <Windows.h>
#include<iostream>
#include "application/application.h"
#include "gpu/gpu.h"
#include "application/image.h"
#include "math/math.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup" )
Image *imagee;
Point p1{ 0, 0, RGBA(255, 0, 0, 255)};
Point p2{ 400, 900, RGBA(0, 255, 0, 255) };
Point p3{ 800, 0, RGBA(0, 0, 255, 255) };
void Prepare() {
	imagee = Image::CreateImage("assets/textures/goku.jpg");
	MALEOON->SetTexture(imagee);
	p1.uv = math::vec2f(0.0f, 0.0f);
	p2.uv = math::vec2f(0.5f, 1.0f);
	p3.uv = math::vec2f(1.0f, 0.0f);
}
void Render() {
	MALEOON->ClearSurfaceView();
	MALEOON->DrawTriangle(p1, p2, p3);
}



/*
* 1 主循环流程在winMain函数中
* 2 窗体构建，消息处理，其他方面的细节都封装在application类里面
*/
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,		//本应用程序实例句柄，唯一指代当前程序
	_In_opt_ HINSTANCE hPrevInstance,	//本程序前一个实例，一般是null
	_In_ LPWSTR    lpCmdLine,		//应用程序运行参数
	_In_ int       nCmdShow)		//窗口如何显示（最大化、最小化、隐藏），不需理会
{
	if (!app->initApplication(hInstance,1024, 960)) {
		return -1;
	}
	MALEOON->InitSurfaceView(app->GetWidth(), app->GetHeight(), app->GetCanvas());

	bool alive = true;
	Prepare();
	while (alive) {
		alive = app->peekMessage();
		Render();
		app->show();
	}

	return 0;
}
