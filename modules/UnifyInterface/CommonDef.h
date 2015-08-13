#pragma once

//Log severity
#define INFO	0
#define WARN	1
#define ERRO	2

//UI const number of device input
//const int DefaultDeviceInput = 8;

//Ini path
#define INIPATH "\\INI\\Unify_Setting.ini"

//enum
enum enumComponents{
	DefaultID = -1, 
	Dev1, Dev2, Dev3, Dev4, Dev5, Dev6, Dev7, Dev8, DevLast
};

//consts
const int DefaultStringLength = 512;
const int DefaultLargeTextSize = 70;
const COLORREF DefaultLargeTextColor = RGB(0, 96, 48);
const COLORREF DefaultErrorTextColor = RGB(196, 0, 0);