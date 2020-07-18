#include<Windows.h>
#include<stdlib.h>
#include <cstdio>
#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define HELP_MENU_ABOUT 4
#define OPEN_FILE 5
#define OPEN_FOLDER 6
#define EDIT_COPY 7
#define EDIT_PASTE 8
#define EDIT_CHANGE_WINDOW_TITLE 9
#define IMAGE_MESSAGE 10
#define OPEN_FILE_DIALOG 11

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HMENU hMenu, image_msg;
HWND hEdit, hLogo, hEditFile;
HWND hname;
HWND hage;
HWND hgen;
HBITMAP himage;

void AddMenus(HWND);
void AddControls(HWND);
void loadImages();
void displayDialog(HWND);
void openFile(HWND);
void displayFile(HWND, char);

int main(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 20, 1000, 1000, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void open_file(HWND hWnd)
{
	OPENFILENAME ofn;
	wchar_t file_name[100];

	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = L"All Files\0*.*\0Text files\0*.txt\0Source files\0*.cpp\0";
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);

	MessageBox(hWnd, ofn.lpstrFile, L"File name", MB_OK);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int val;
	switch(msg)
	{
	case WM_COMMAND:
		switch (wp)
		{
		case FILE_MENU_EXIT:
			val = MessageBoxW(hWnd, L"Are you sure you want to quit?", L"Really quit", MB_YESNO | MB_ICONEXCLAMATION);
			if(val == IDYES)
				DestroyWindow(hWnd);
						break;
		case OPEN_FILE_DIALOG:
			open_file(hWnd);
			break;

		case FILE_MENU_NEW:
			MessageBeep(MB_HELP);
			break;
		case EDIT_CHANGE_WINDOW_TITLE:

			char name[30], age[20], out[50];
			GetWindowTextA(hname, name, 30);
			GetWindowTextA(hage, age, 30);

			if (strcmp(name, "") == 0 || strcmp(age, "") == 0)
			{
				val = MessageBoxW(hWnd, L"You did not enter anything!", NULL, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION);
				switch(val)
				{
				case IDABORT:
					DestroyWindow(hWnd);
					break;
				case IDRETRY:
					return 0;
				case IDIGNORE:
					break;
				}
			}
			strcpy_s(out, name);
			strcat_s(out, " is ");
			strcat_s(out, age);
			strcat_s(out, " years old.");

			SetWindowTextA(hgen, out);
			break;
		case IMAGE_MESSAGE:
			MessageBox(hWnd, L"Dont press it", L"You pressed the image", MB_OK);
			break;
		}
		break;
	case WM_CREATE:
		openFile(hWnd);
		//displayDialog(hWnd);
		loadImages();
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
} 

void AddMenus(HWND hWnd)
{
	hMenu = CreateMenu();

	HMENU hFileMenu = CreateMenu();
	HMENU hHelpMenu = CreateMenu();
	HMENU hOpenMenu = CreateMenu();
	HMENU hEditMenu = CreateMenu();

	AppendMenu(hOpenMenu, MF_STRING, OPEN_FILE, L"Open File");
	AppendMenu(hOpenMenu, MF_STRING, OPEN_FOLDER, L"Open Folder");

	AppendMenu(hHelpMenu, MF_STRING, HELP_MENU_ABOUT, L"About");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hOpenMenu, L"Open");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

	AppendMenu(hEditMenu, MF_STRING, NULL, L"Change Window Title");
	AppendMenu(hEditMenu, MF_STRING, EDIT_COPY, L"Copy");
	AppendMenu(hEditMenu, MF_STRING, EDIT_PASTE, L"Paste");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, L"Edit");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"Help");


	SetMenu(hWnd, hMenu);
}


void AddControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Name", WS_VISIBLE | WS_CHILD, 100, 100, 100, 50, hWnd, NULL, NULL, NULL);
	hname = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 100, 100, 50, hWnd, NULL, NULL, NULL);
	
	CreateWindowW(L"static", L"Age", WS_VISIBLE | WS_CHILD, 100, 200, 100, 50, hWnd, NULL, NULL, NULL);
	hage = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 200, 100, 50, hWnd, NULL, NULL, NULL);

	CreateWindowW(L"button", L"Generate", WS_VISIBLE | WS_CHILD, 150, 300, 150, 50, hWnd, (HMENU)EDIT_CHANGE_WINDOW_TITLE, NULL, NULL);
	hgen = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD, 75, 400, 350, 200, hWnd, NULL, NULL, NULL);

	hLogo = CreateWindowW(L"button", NULL, WS_VISIBLE | WS_CHILD | BS_BITMAP, 500, 300, 300, 300, hWnd, (HMENU)IMAGE_MESSAGE, NULL, NULL);
	SendMessageW(hLogo, BM_SETIMAGE, (IMAGE_BITMAP), (LPARAM)himage);
}

void loadImages()
{
	himage = (HBITMAP)LoadImageW(NULL, L"LAND2.BMP", IMAGE_BITMAP, 300, 300, LR_LOADFROMFILE);
}

LRESULT CALLBACK dialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}

void createDialogClass(HINSTANCE hInst)
{
	WNDCLASSW dialog = { 0 };
	dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
	dialog.hInstance = hInst;
	dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
	dialog.lpszClassName = L"myDialogClass";
	dialog.lpfnWndProc = dialogProcedure;

	RegisterClassW(&dialog);
}

void displayDialog(HWND hWnd)
{
	HWND hdlg = CreateWindowW(L"myDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400, 400, 100, 100, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"buttonclass", WS_VISIBLE | WS_CHILD, 20, 20, 100, 40, hdlg, (HMENU)1, NULL, NULL);
	EnableWindow(hWnd, false);
}

void openFile(HWND hWnd)
{
	CreateWindowW(L"button", L"Open File", WS_VISIBLE | WS_CHILD, 100, 800, 100, 30, hWnd, (HMENU)OPEN_FILE_DIALOG, NULL, NULL);
	hEditFile = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOHSCROLL, 220, 800, 350, 30, hWnd, NULL, NULL, NULL);

}

//void displayFile(HWND hWnd, char* path)
//{
//	FILE* file;
//	file = fopen(path, "rb");
//	fseek(file, 0, SEEK_END);
//	int _size = ftell(file);
//	rewind(file);
//	wchar_t data[1000];
//	fread(data, _size, 1, file);
//	data[_size + 1] = '\0';
//
//	SetWindowText(hEditFile, data);
//}
