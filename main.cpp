#include <Windows.h>
#include<iostream>
#include "application/application.h"
#include "gpu/gpu.h"
#include "application/image.h"
#include "math/mathfunctions.h"
#include "math/matrix.h"

#pragma comment(linker, "/subsystem:console /entry:wWinMainCRTStartup" )

//三个属性对应vbo
uint32_t positionVbo = 0;
uint32_t colorVbo = 0;
uint32_t uvVbo = 0;

//三角形的indices
uint32_t ebo = 0;

//本三角形专属vao
uint32_t vao = 0;
void Prepare() {
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
	};
	uint32_t indices[] = { 0, 1, 2 };

	ebo = MALEOON->GenBuffer();
	MALEOON->BindBuffer(ELEMENT_ARRAY_BUFFER, ebo);
	MALEOON->BufferData(ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
	MALEOON->BindBuffer(ELEMENT_ARRAY_BUFFER, 0);

	vao = MALEOON->GenVertexArray();
	MALEOON->BindVertexArray(vao);
	auto positionVBO = MALEOON->GenBuffer();
	MALEOON->BindBuffer(ARRAY_BUFFER, positionVBO);
	MALEOON->BufferData(ARRAY_BUFFER, sizeof(positions), positions);
	MALEOON->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);

	auto colorVBO = MALEOON->GenBuffer();
	MALEOON->BindBuffer(ARRAY_BUFFER, colorVBO);
	MALEOON->BufferData(ARRAY_BUFFER, sizeof(colors), colors);
	MALEOON->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);

	auto uvVBO = MALEOON->GenBuffer();
	MALEOON->BindBuffer(ARRAY_BUFFER, uvVBO);
	MALEOON->BufferData(ARRAY_BUFFER, sizeof(uvs), uvs);
	MALEOON->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);

	MALEOON->BindBuffer(ARRAY_BUFFER, 0);
	MALEOON->BindVertexArray(0);

	MALEOON->PrintVao(vao);

}
void Render() {
	MALEOON->ClearSurfaceView();
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
