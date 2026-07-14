#include <Windows.h>
#include<iostream>
#include "application/application.h"
#include "gpu/gpu.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup" )

void Render() {
	MALEOON->ClearSurfaceView();
	// for (int i = 0; i < app->GetWidth(); i++) {
	// 	for (int j = 0; j < app->GetHeight(); j++) {
	// 		uint32_t v = std::rand() % 255;
	// 		RGBA color(v, v, v, v);
	// 		MALEOON->DrawPoint(i, j, color);
	// 	}
	// }
	// Point start = Point(100 ,500);
	// start.color = RGBA{0, 255, 0, 0};
	// Point end = Point(200, 0);
	// end.color = RGBA{0, 0, 255, 0};
	// MALEOON->DrawLine(start, end);

	// int r = 150;
	// Point c{400, 300, RGBA(255, 0, 0, 255)};
	//
	// for (float i = 0; i < 360; i += 10) {
	// 	float radian = DEG2RAD(i);
	// 	int x = r * sin(radian) + c.x;
	// 	int y = r * cos(radian) + c.y;
	//
	// 	Point pt{x, y, RGBA(rand() % 255, rand() % 255, rand() % 255, 255)};
	//
	// 	MALEOON->DrawLine(c, pt);
	// }

	// 绘制三角形
	// Point p1{ 0, 100, RGBA(255, 0, 0, 255)};
	// Point p2{ 500, 100, RGBA(0, 255, 0, 255) };
	// Point p3{ 250, 500, RGBA(0, 0, 255, 255) };
	// MALEOON->DrawTriangle(p1, p2, p3);
	Image *image = Image::CreateImage("assets/textures/goku.jpg"); //
	MALEOON->SetEnableBlending(true);
	// MALEOON->DrawImage(image);
	MALEOON->DrawImageWithAlpha(image, 100);
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
	if (!app->initApplication(hInstance,800, 600)) {
		return -1;
	}
	MALEOON->InitSurfaceView(app->GetWidth(), app->GetHeight(), app->GetCanvas());

	bool alive = true;
	while (alive) {
		alive = app->peekMessage();
		Render();
		app->show();
	}

	return 0;
}
