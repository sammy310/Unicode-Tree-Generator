#include <Windows.h>
#include <ShlObj.h>
#include <list>
#include <filesystem>
namespace fs = std::experimental::filesystem;

#define ID_EDIT_TREE 1
#define ID_EDIT_SAVE 2
#define ID_EDIT_SAVEFILE 3
#define ID_GETTREEBUTTON 11
#define ID_EXITBUTTON 12

#define NEW_LINE TEXT("\r\n")

HINSTANCE g_hInst;
HWND g_hWnd;
LPCWSTR lpszClass = TEXT("Tree");
LPCWSTR lpszMenuName = TEXT("Unicode Tree Generator");
int windowWidth = 500;
int windowHeight = 300;

HWND g_hEditTree;
HWND g_hEditSave;
HWND g_hEditSaveFile;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL GetOpenFile(WCHAR*);
BOOL GetDirectory(WCHAR*);

struct TreeStruct {
	fs::path ownPath;
	std::list<TreeStruct> dirList;
	std::list<fs::path> fileList;
};
BOOL CreateTree(LPCWSTR, LPCWSTR, LPCWSTR);
void CreateTreeLoop(TreeStruct*);
void WriteTree(HANDLE, TreeStruct*, std::wstring, bool);




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDC_ICON);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = lpszMenuName;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(
		lpszClass,
		lpszMenuName,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2,
		windowWidth,
		windowHeight,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL
	);
	ShowWindow(hWnd, nCmdShow);
	g_hWnd = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	static WCHAR treePath[MAX_PATH] = TEXT("");
	static WCHAR savePath[MAX_PATH] = TEXT("");
	static WCHAR saveFile[260] = TEXT("");
	switch (iMessage) {
	case WM_CREATE:
		g_hEditTree = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, windowWidth / 2 - 90, 50, 300, 30, hWnd, (HMENU)ID_EDIT_TREE, g_hInst, NULL);
		CreateWindow(TEXT("static"), TEXT("Extract Tree Path :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_LEFT, windowWidth / 2 - 250, 50, 150, 30, hWnd, (HMENU)-1, g_hInst, NULL);
		g_hEditSave = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, windowWidth / 2 - 90, 90, 300, 30, hWnd, (HMENU)ID_EDIT_SAVE, g_hInst, NULL);
		CreateWindow(TEXT("static"), TEXT("Save File Path :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_LEFT, windowWidth / 2 - 250, 90, 150, 30, hWnd, (HMENU)-1, g_hInst, NULL);
		g_hEditSaveFile = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, windowWidth / 2 - 90, 130, 300, 30, hWnd, (HMENU)ID_EDIT_SAVEFILE, g_hInst, NULL);
		CreateWindow(TEXT("static"), TEXT("File Name :"), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_LEFT, windowWidth / 2 - 250, 130, 150, 30, hWnd, (HMENU)-1, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("Generate"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, windowWidth / 2 - 100, windowHeight - 110, 80, 30, hWnd, (HMENU)ID_GETTREEBUTTON, g_hInst, NULL);
		CreateWindow(TEXT("button"), TEXT("Exit"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, windowWidth / 2 + 20, windowHeight - 110, 80, 30, hWnd, (HMENU)ID_EXITBUTTON, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_EDIT_TREE:
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				GetDirectory(treePath);
				SetWindowText(g_hEditTree, treePath);
				break;
			}
			break;

		case ID_EDIT_SAVE:
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				GetDirectory(savePath);
				SetWindowText(g_hEditSave, savePath);
				break;
			}
			break;

		case ID_EDIT_SAVEFILE:
			switch (HIWORD(wParam)) {
			case EN_KILLFOCUS:
				GetWindowText(g_hEditSaveFile, saveFile, 260);
				break;
			}
			break;

		case ID_GETTREEBUTTON:
			if (CreateTree(treePath, savePath, saveFile) == TRUE)
				MessageBox(hWnd, TEXT("Generate Complete!!"), TEXT("!"), MB_OK);
			break;

		case ID_EXITBUTTON:
			PostQuitMessage(0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}


BOOL GetOpenFile(WCHAR *fileStr) {
	OPENFILENAME OFN;
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = g_hWnd;
	OFN.lpstrFilter = TEXT("Æú´õ\0*\0");
	OFN.lpstrFile = fileStr;
	OFN.nMaxFile = 256;
	OFN.lpstrInitialDir = TEXT("c\\");
	return GetOpenFileName(&OFN);
}



BOOL GetDirectory(WCHAR *pathStr) {
	BROWSEINFO bi;
	LPITEMIDLIST dirList;
	WCHAR nameStr[MAX_PATH];

	memset(&bi, 0, sizeof(bi));
	bi.hwndOwner = g_hWnd;
	bi.pszDisplayName = nameStr;
	bi.lpszTitle = TEXT("Path");
	bi.ulFlags = BIF_VALIDATE | BIF_USENEWUI | BIF_NONEWFOLDERBUTTON | BIF_RETURNONLYFSDIRS;
	bi.pidlRoot=

	dirList=SHBrowseForFolder(&bi);
	if (dirList == NULL)return FALSE;

	SHGetPathFromIDList(dirList, pathStr);

	LPMALLOC pMalloc = NULL;
	SHGetMalloc(&pMalloc);
	pMalloc->Free(dirList);
	pMalloc->Release();
	return TRUE;
}


BOOL CreateTree(LPCWSTR pathDir, LPCWSTR savePath, LPCWSTR saveFileName) {
	
	if (fs::exists(pathDir) == false || fs::exists(savePath) == false) return FALSE;

	std::wstring saveFile = saveFileName;
	if (saveFile.size()==0) {
		saveFile.append(TEXT("Tree"));
	}


	std::wstring saveFilePath = savePath;
	saveFilePath.append(TEXT("\\"));
	saveFilePath.append(saveFile);
	saveFilePath.append(TEXT(".txt"));
	HANDLE hFile = CreateFile(saveFilePath.c_str() , GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WCHAR szTmp[2] = { 0xfeff };
	WriteFile(hFile, szTmp, wcslen(szTmp) * sizeof(WCHAR), NULL, NULL);

	TreeStruct tree;
	fs::path path = pathDir;
	tree.ownPath = path;

	CreateTreeLoop(&tree);

	WriteTree(hFile, &tree, TEXT(""), false);
	CloseHandle(hFile);
	return TRUE;
}

void CreateTreeLoop(TreeStruct* tree) {
	fs::path newPath;

	for (auto &p : fs::directory_iterator(tree->ownPath)) {
		newPath = p.path();
		if (fs::is_directory(p.status())) {
			TreeStruct newTree;
			newTree.ownPath = newPath;
			tree->dirList.push_back(newTree);

			CreateTreeLoop(&tree->dirList.back());
		}
		else {
			tree->fileList.push_back(newPath);
		}
	}
}

void WriteTree(HANDLE hFile, TreeStruct* tree, std::wstring treeStr, bool isLastDir) {
	std::wstring str;
	std::wstring newTreeStr = treeStr;

	if (treeStr.size() >= 3) {
		newTreeStr.erase(newTreeStr.size() - 3, 3);
		if (isLastDir == false) {
			newTreeStr.append(TEXT("¦§¦¡"));
		}
		else {
			newTreeStr.append(TEXT("¦¦¦¡"));
		}
	}
	str = newTreeStr;
	str.append(tree->ownPath.filename().wstring());
	str.append(NEW_LINE);
	WriteFile(hFile, str.c_str(), (str.length()) * sizeof(WCHAR), NULL, NULL);


	newTreeStr = treeStr;
	if (isLastDir == true) {
		newTreeStr.erase(newTreeStr.size() - 3, 3);
		if (tree->dirList.size() > 0) {
			newTreeStr.append(TEXT("    ¦¢  "));
		}
		else {
			newTreeStr.append(TEXT("        "));
		}
	}
	else {
		if (tree->dirList.size() > 0) {
			newTreeStr.append(TEXT("¦¢  "));
		}
		else {
			newTreeStr.append(TEXT("    "));
		}
	}

	//file
	std::list<fs::path>::iterator iter;
	for (iter = tree->fileList.begin(); iter != tree->fileList.end(); iter++) {
		str = newTreeStr;

		str.append(iter->filename().wstring());
		str.append(NEW_LINE);
		WriteFile(hFile, str.c_str(), (str.length()) * sizeof(WCHAR), NULL, NULL);
	}
	if (tree->fileList.size() > 0) {
		str = newTreeStr;
		str.append(NEW_LINE);
		WriteFile(hFile, str.c_str(), (str.length()) * sizeof(WCHAR), NULL, NULL);
	}

	//directory
	std::list<TreeStruct>::iterator treeIter;
	for (treeIter = tree->dirList.begin(); treeIter != tree->dirList.end(); treeIter++) {
		bool isLast = (std::next(treeIter) == tree->dirList.end() ? true : false);
		WriteTree(hFile, &(*treeIter), newTreeStr, isLast);
	}
}