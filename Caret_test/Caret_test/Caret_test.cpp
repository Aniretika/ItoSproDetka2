#include <windows.h> // підключення бібліотеки з функціями API

// Глобальні змінні:
HINSTANCE hInst; //Дескриптор програми
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "ДРУГА ПРОГРАМА";
// Попередній опис функцій
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TEXTMETRIC tm;
// Основна програма
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow)
{
	MSG msg;
	// Реєстрація класу вікна
	MyRegisterClass(hInstance);
	// Створення вікна програми
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обробки повідомлень
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //стиль вікна
	wcex.lpfnWndProc = (WNDPROC)WndProc; //віконна процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance; //дескриптор програми
	wcex.hIcon = LoadIcon(NULL, IDI_HAND); //визначення іконки
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //визначення курсору
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW + 1); //установка фону
	wcex.lpszMenuName = NULL; //визначення меню
	wcex.lpszClassName = szWindowClass; //ім’я класу
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex); //реєстрація класу вікна
}
// FUNCTION: InitInstance (HANDLE, int)
// Створює вікно програми і зберігає дескриптор програми в змінній hInst
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; //зберігає дескриптор додатка в змінній hInst
	hWnd = CreateWindow(szWindowClass, // ім’я класу вікна
		szTitle, // назва програми
		WS_OVERLAPPEDWINDOW, // стиль вікна
		CW_USEDEFAULT, // положення по Х
		CW_USEDEFAULT, // положення по Y
		900, // розмір по Х
		900, // розмір по Y
		NULL, // дескриптор батьківського вікна
		NULL, // дескриптор меню вікна
		hInstance, // дескриптор програми
		NULL); // параметри створення.
	if (!hWnd) //Якщо вікно не творилось, функція повертає FALSE
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow); //Показати вікно
	UpdateWindow(hWnd); //Оновити вікно
	return TRUE;
}





// FUNCTION: WndProc (HWND, unsigned, WORD, LONG)
// Віконна процедура. Приймає і обробляє всі повідомлення, що приходять в додаток

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	hdc = GetDC(hWnd);
	static char myString[100];
	static char myString1[10][100];
	static int nNumOfSymbs = 0; //размер заполняемого массива
	static WORD xPos(110), yPos(5);    //позиція курсора на екрані
	int x = 10;
	int y = 150;
	char buffer[100];

	RECT rect;
	GetWindowRect(hWnd, &rect);
	LONG width = rect.right - rect.left;

	GetTextMetrics(hdc, &tm);

	switch (message)
	{

	case WM_CHAR:
	{
		if (nNumOfSymbs < sizeof(myString))
		{
			myString[nNumOfSymbs++] = (char)wParam;
			myString[nNumOfSymbs] = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		if (nNumOfSymbs >= 99)
		{
			MessageBox(hWnd, "На жаль, ви перевищили ліміт введених символів. Нажміть 'Ок', а потім на вільне місце в екрані, щоб продовжити.", "У вас повідомлення" , MB_OK);
			nNumOfSymbs--;
		}
	}

	case WM_CREATE: //Повідомлення приходить при створенні вікна
		break;


	case  WM_LBUTTONDOWN:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		for (int i = 9; i > 0; i--) {
			memcpy(myString1[i], myString1[i - 1], 100);//копирует первый(ввденный) элемент массива в следующий
		}
		memcpy(myString1[0], myString, 100);

		memset(myString, 0, 100);
		/*
		void * memset ( void * ptr, int value, size_t num );

		***Устанавливает для первого числа байтов блока памяти, на который указывает ptr,
		указанное значение (интерпретируемое как символ без знака).
		*** ptr - Указатель на блок памяти для заполнения.
		**** value - Значение, которое нужно установить. Значение передается как int, но функция заполняет блок памяти,
		используя беззнаковое преобразование этого значения в char.
		**** num - Количество байтов, которое нужно установить в значение.
		size_t - это целочисленный тип без знака.
		возвращает  ptr
		*/
		nNumOfSymbs = 0; //обнуление индекса заполняемого массива

		break;

	case WM_PAINT:         //Перемалювати вікно

		hdc = BeginPaint(hWnd, &ps);   //Почати графічний вивід
		
		TextOut(hdc, 10, 10, buffer, wsprintf(buffer, "Ширина екрана : %2d", GetDeviceCaps(hdc, HORZRES)));
		TextOut(hdc, 10, 30, buffer, wsprintf(buffer, "Висота екрана : %2d", GetDeviceCaps(hdc, VERTRES)));
		TextOut(hdc, 10, 50, buffer, wsprintf(buffer, "Ширина рамки окна : %2d", width));
		TextOut(hdc, 10, 70, buffer, wsprintf(buffer, "Ширина іконки : %2d", GetSystemMetrics(SM_CXICON)));
		TextOut(hdc, 10, 90, buffer, wsprintf(buffer, "Висота іконки : %2d", GetSystemMetrics(SM_CYICON)));
		TextOut(hdc, 10, 110, buffer, wsprintf(buffer, "Висота символів : %2d", tm.tmHeight));
		TextOut(hdc, 10, 130, buffer, wsprintf(buffer, "Міжстроковий інтервал : %2d", tm.tmAscent));

		GetClientRect(hWnd, &rt);     //Область вікна для малювання
		
		TextOut(hdc, xPos, yPos, myString, nNumOfSymbs);


		for (int i = 0; i < 10; i++) {
			TextOut(hdc, x, y, myString1[i], strlen(myString1[i])); //strlen(myString1[i] рассчитывает длину введенных символов пользователем
			y += 20;
		}


		EndPaint(hWnd, &ps);     //Закінчити графічний вивід  
		break;

	case WM_DESTROY:         //Завершення роботи
		PostQuitMessage(0);
		break;

	default:
		//Обробка повідомлень, які не оброблені користувачем
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}