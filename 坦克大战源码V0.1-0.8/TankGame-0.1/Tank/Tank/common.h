#pragma once
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <tchar.h>

#include <easyx.h>			// ����ͼ�ο�ͷ�ļ�

#include <windows.h>
#include <Mmsystem.h>
#pragma comment ( lib, "Winmm.lib" )

// GDI
#include <wingdi.h>
#pragma comment (lib, "msimg32.lib")

#define WINDOW_WIDTH 512 // ���崰�ڴ�С
#define WINDOW_HEIGHT 448
#define CANVAS_WIDTH	256		// ���� image ��С������ı䣬���һ�ͼ���� image �ϲ�����Ȼ��һ���Ի��Ƶ�������
#define CANVAS_HEIGHT	224