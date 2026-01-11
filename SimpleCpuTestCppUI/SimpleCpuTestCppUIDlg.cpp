// SimpleCpuTestCppUIDlg.cpp : implementation file
#include "pch.h"
#include "framework.h"
#include "SimpleCpuTestCppUI.h"
#include "SimpleCpuTestCppUIDlg.h"
#include "afxdialogex.h"

#include <vector>
#include <string>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <functional> // Виправляє помилку з std::function
#include <thread>     // Виправляє помилку з std::thread

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Константи для бенчмарку
const long long INT_OPERATIONS = 500000000;
const long long FLOAT_OPERATIONS = 250000000;
const long long FUNC_OPERATIONS = 5000000;
const long long LOGICAL_OPERATIONS = 500000000;

// Допоміжні функції для логіки
std::wstring GetCpuVendor()
{
    // Простий спосіб отримати vendor string через environment variable (як у C# версії)
    size_t requiredSize;
    _wgetenv_s(&requiredSize, nullptr, 0, L"PROCESSOR_IDENTIFIER");
    if (requiredSize == 0) return L"UnknownCPU";

    std::vector<wchar_t> buffer(requiredSize);
    _wgetenv_s(&requiredSize, buffer.data(), requiredSize, L"PROCESSOR_IDENTIFIER");

    std::wstring res(buffer.data());
    size_t commaPos = res.find(L',');
    if (commaPos != std::wstring::npos)
        return res.substr(0, commaPos);
    return res;
}

// Рекурсивний факторіал
long double Factorial(long long n)
{
    if (n <= 1) return 1.0;
    return n * Factorial(n - 1);
}

CSimpleCpuTestCppUIDlg::CSimpleCpuTestCppUIDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SIMPLECPUTESTCPPUI_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleCpuTestCppUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_START, m_btnStart);
    DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
    DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
}

BEGIN_MESSAGE_MAP(CSimpleCpuTestCppUIDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_START, &CSimpleCpuTestCppUIDlg::OnBnClickedBtnStart)
    ON_WM_CTLCOLOR() // Для кольорів
    // Реєстрація наших повідомлень
    ON_MESSAGE(WM_APP_LOG_UPDATE, &CSimpleCpuTestCppUIDlg::OnLogUpdate)
    ON_MESSAGE(WM_APP_PROGRESS_UPDATE, &CSimpleCpuTestCppUIDlg::OnProgressUpdate)
    ON_MESSAGE(WM_APP_BENCHMARK_FINISHED, &CSimpleCpuTestCppUIDlg::OnBenchmarkFinished)
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CSimpleCpuTestCppUIDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    // 1. Налаштування шрифту (Consolas, 10pt)
    m_fontLog.CreatePointFont(100, L"Consolas");
    m_editLog.SetFont(&m_fontLog);

    // 2. Створення чорного пензля для фону
    m_brushBack.CreateSolidBrush(RGB(0, 0, 0));

    int myWidth = 800;
    int myHeight = 600;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int x = (screenWidth - myWidth) / 2;
    int y = (screenHeight - myHeight) / 2;

    MoveWindow(x, y, myWidth, myHeight);

    return TRUE;
}

// Зміна кольорів (Чорний фон, Зелений текст)
HBRUSH CSimpleCpuTestCppUIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd->GetDlgCtrlID() == IDC_EDIT_LOG)
    {
        pDC->SetTextColor(RGB(0, 255, 0)); // Зелений текст
        pDC->SetBkColor(RGB(0, 0, 0));     // Чорний фон тексту
        return m_brushBack;                // Чорний фон контролу
    }

    return hbr;
}

void CSimpleCpuTestCppUIDlg::OnBnClickedBtnStart()
{
    m_btnStart.EnableWindow(FALSE);
    m_editLog.SetWindowText(L"");
    m_progressBar.SetPos(0);

    // Запуск робочого потоку
    AfxBeginThread(BenchmarkThread, this);
}

// --- Методи для оновлення UI (виконуються в головному потоці) ---

LRESULT CSimpleCpuTestCppUIDlg::OnLogUpdate(WPARAM wParam, LPARAM lParam)
{
    // Отримуємо рядок з потоку
    CString* pMsg = (CString*)lParam;

    // Отримуємо поточну довжину тексту
    int len = m_editLog.GetWindowTextLength();

    // Ставимо курсор в самий кінець
    m_editLog.SetSel(len, len);

    // Вставляємо текст (це автоматично прокрутить до курсору)
    m_editLog.ReplaceSel(*pMsg);

    // --- ДОДАТКОВА ПРИМУСОВА ПРОКРУТКА ---
    // Іноді ReplaceSel не прокручує до кінця, якщо вікно не активне.
    // Ця команда гарантовано прокрутить вниз.
    m_editLog.LineScroll(m_editLog.GetLineCount());
    // -------------------------------------

    // Видаляємо пам'ять
    delete pMsg;
    return 0;
}

LRESULT CSimpleCpuTestCppUIDlg::OnProgressUpdate(WPARAM wParam, LPARAM lParam)
{
    int progress = (int)wParam;
    m_progressBar.SetPos(progress);
    return 0;
}

LRESULT CSimpleCpuTestCppUIDlg::OnBenchmarkFinished(WPARAM wParam, LPARAM lParam)
{
    m_btnStart.EnableWindow(TRUE);
    MessageBox(L"Benchmark Completed!", L"Done", MB_OK | MB_ICONINFORMATION);
    return 0;
}

// --- РОБОЧИЙ ПОТІК (Логіка Бенчмарку) ---

CString GenerateLogFileName()
{
    // Отримуємо поточний час
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Отримуємо інформацію про CPU
    CString cpuInfo = GetCpuVendor().c_str();

    // Видаляємо недопустимі символи з імені файлу (якщо є)
    CString invalidChars = L"\\/:*?\"<>|";
    for (int i = 0; i < invalidChars.GetLength(); i++)
    {
        cpuInfo.Replace(invalidChars[i], L'_');
    }

    // Формуємо ім'я: Benchmark_YYYY-MM-DD_HH-mm-ss_CpuVendor.log
    CString fileName;
    fileName.Format(L"Benchmark_%04d-%02d-%02d_%02d-%02d-%02d_%s.log",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, cpuInfo);

    return fileName;
}

// Допоміжний клас для зручного логування з потоку
class ThreadLogger
{
    HWND m_hWnd;
    CStdioFile m_file;
    bool m_isFileOpen;

public:
    ThreadLogger(HWND hWnd) : m_hWnd(hWnd), m_isFileOpen(false)
    {
        // Генеруємо ім'я та відкриваємо файл
        CString fileName = GenerateLogFileName();

        CFileException ex;
        // Відкриваємо для запису (Create), записуємо текст (Text), дозволяємо іншим читати (ShareDenyWrite)
        if (m_file.Open(fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
        {
            m_isFileOpen = true;

            // (Опціонально) Записуємо BOM (Byte Order Mark) для Unicode, 
            // щоб блокнот точно зрозумів, що це UTF-16 LE
            const unsigned char bom[] = { 0xFF, 0xFE };
            m_file.Write(bom, 2);
        }
        else
        {
            // Якщо не вдалося відкрити файл, можна повідомити про помилку, 
            // але ми просто будемо писати тільки в UI.
            TCHAR szError[1024];
            ex.GetErrorMessage(szError, 1024);
            ::OutputDebugString(szError); // Вивід у вікно Output в Visual Studio
        }
    }

    ~ThreadLogger()
    {
        if (m_isFileOpen)
        {
            m_file.Close();
        }
    }

    void Log(const CString& msg, bool newLine = true)
    {
        // 1. Відправляємо в UI
        CString* pStr = new CString(msg);
        if (newLine) *pStr += L"\r\n";
        ::PostMessage(m_hWnd, WM_APP_LOG_UPDATE, 0, (LPARAM)pStr);

        // 2. Пишемо у файл
        if (m_isFileOpen)
        {
            try
            {
                m_file.WriteString(msg);
                if (newLine) m_file.WriteString(L"\n");
            }
            catch (CFileException* e)
            {
                e->Delete();
                m_isFileOpen = false; // Перестаємо писати при помилці
            }
        }
    }

    void LogInline(const CString& msg)
    {
        Log(msg, false);
    }
};

UINT __cdecl CSimpleCpuTestCppUIDlg::BenchmarkThread(LPVOID pParam)
{
    CSimpleCpuTestCppUIDlg* pDlg = (CSimpleCpuTestCppUIDlg*)pParam;
    HWND hWnd = pDlg->GetSafeHwnd();
    ThreadLogger logger(hWnd);

    std::vector<double> allMipsResults;
    auto totalStart = std::chrono::high_resolution_clock::now();

    // --- Header ---
    SYSTEMTIME st;
    GetLocalTime(&st);
    CString header;
    header.Format(L"*** BENCHMARK TEST AT %02d Hour %02d Min %02d Sec %03d Thousands ***",
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    logger.Log(header);
    logger.Log(L"");

    CString cpuInfo;
    cpuInfo.Format(L"> Processor: %d Cores. Vendor: %s", std::thread::hardware_concurrency(), GetCpuVendor().c_str());
    logger.Log(cpuInfo);
    logger.Log(L"> Runtime: Native C++ (MFC)");
    logger.Log(L"");

    int totalSteps = 7;
    int currentStep = 0;

    auto RunMipsTest = [&](const wchar_t* name, long long ops, std::function<void()> func) -> double {
        CString msg;
        msg.Format(L"· %-45s", name);
        logger.LogInline(msg);

        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        double mips = (seconds > 0) ? (ops / seconds) / 1000000.0 : 0;

        msg.Format(L": %.6f MIPS", mips);
        logger.Log(msg);
        allMipsResults.push_back(mips);
        return mips;
        };

    auto PrintAverage = [&](const wchar_t* title, const std::vector<double>& results) {
        double sum = std::accumulate(results.begin(), results.end(), 0.0);
        double avg = results.empty() ? 0.0 : sum / results.size();
        CString msg;
        msg.Format(L"· %s Average: %.6f MIPS.", title, avg);
        logger.Log(msg);
        logger.Log(L"");
        };

    // --- 1. Integer Tests ---
    logger.Log(L"* INTEGER ARITHMETIC:");
    std::vector<double> intResults;

    int8_t i8_1 = 10, i8_2 = 5, i8_res = 0;
    intResults.push_back(RunMipsTest(L"Execute 8 bit Additions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i8_res = i8_1 + i8_2; }));
    intResults.push_back(RunMipsTest(L"Execute 8 bit Subtractions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i8_res = i8_1 - i8_2; }));
    intResults.push_back(RunMipsTest(L"Execute 8 bit Multiplications", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i8_res = i8_1 * i8_2; }));
    intResults.push_back(RunMipsTest(L"Execute 8 bit Divisions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i8_res = i8_1 / i8_2; }));
    PrintAverage(L"8 bit Integer", intResults);
    intResults.clear();

    int32_t i32_1 = 10, i32_2 = 5, i32_res = 0;
    intResults.push_back(RunMipsTest(L"Execute 32 bit Additions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i32_res = i32_1 + i32_2; }));
    intResults.push_back(RunMipsTest(L"Execute 32 bit Subtractions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i32_res = i32_1 - i32_2; }));
    intResults.push_back(RunMipsTest(L"Execute 32 bit Multiplications", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i32_res = i32_1 * i32_2; }));
    intResults.push_back(RunMipsTest(L"Execute 32 bit Divisions", INT_OPERATIONS, [&]() { for (long long i = 0; i < INT_OPERATIONS; ++i) i32_res = i32_1 / i32_2; }));
    PrintAverage(L"32 bit Integer", intResults);

    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 2. Floating Point ---
    logger.Log(L"* FLOATING POINT ARITHMETIC:");
    std::vector<double> floatResults;

    float f1 = 1.23f, f2 = 4.56f, fRes = 0;
    floatResults.push_back(RunMipsTest(L"Execute 32 bit (Single) Additions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; ++i) fRes = f1 + f2; }));
    floatResults.push_back(RunMipsTest(L"Execute 32 bit (Single) Multiplications", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; ++i) fRes = f1 * f2; }));

    double d1 = 1.23, d2 = 4.56, dRes = 0;
    floatResults.push_back(RunMipsTest(L"Execute 64 bit (Double) Additions", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; ++i) dRes = d1 + d2; }));
    floatResults.push_back(RunMipsTest(L"Execute 64 bit (Double) Multiplications", FLOAT_OPERATIONS, [&]() { for (long long i = 0; i < FLOAT_OPERATIONS; ++i) dRes = d1 * d2; }));
    PrintAverage(L"Floating Point", floatResults);

    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 3. Functions ---
    logger.Log(L"* MATH FUNCTIONS:");
    std::vector<double> funcResults;
    double num = 1234567890.0;
    funcResults.push_back(RunMipsTest(L"Square Root", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; ++i) dRes = sqrt(num); }));
    funcResults.push_back(RunMipsTest(L"Sinus", FUNC_OPERATIONS, [&]() { for (long long i = 0; i < FUNC_OPERATIONS; ++i) dRes = sin(0.785); }));
    PrintAverage(L"Math Functions", funcResults);

    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 4. Logical ---
    logger.Log(L"* LOGICAL INSTRUCTIONS:");
    std::vector<double> logicResults;
    int v1 = 0xAAAAAAAA, v2 = 0x55555555, vRes = 0;
    logicResults.push_back(RunMipsTest(L"Logical AND", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; ++i) vRes = v1 & v2; }));
    logicResults.push_back(RunMipsTest(L"Logical OR", LOGICAL_OPERATIONS, [&]() { for (long long i = 0; i < LOGICAL_OPERATIONS; ++i) vRes = v1 | v2; }));
    PrintAverage(L"Logical", logicResults);

    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 5. Recursion ---
    logger.Log(L"* RECURSIVE FACTORIAL:");
    long double factRes = 0;
    RunMipsTest(L"17 Factorial", 1, [&]() { factRes = Factorial(17); });
    logger.Log(L"");
    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 6. Sorting ---
    logger.Log(L"* QUICK SORT (std::sort):");
    int stringCount = 80000;
    int stringLength = 80;
    logger.LogInline(L"· Sorting 80000 strings... ");

    auto tStart = std::chrono::high_resolution_clock::now();
    {
        std::vector<std::string> data(stringCount);
        for (int i = 0; i < stringCount; ++i) {
            data[i].resize(stringLength);
            for (int j = 0; j < stringLength; ++j) data[i][j] = 'A' + (rand() % 26);
        }
        std::sort(data.begin(), data.end());
    }
    auto tEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tElapsed = tEnd - tStart;

    CString timeMsg;
    timeMsg.Format(L"Elapsed: %.0f ms", tElapsed.count());
    logger.Log(timeMsg);
    logger.Log(L"");
    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(++currentStep * 100 / totalSteps), 0);

    // --- 7. Primes ---
    logger.Log(L"* PRIME NUMBERS:");
    int limit = 800000;
    int primeCount = 0;
    logger.LogInline(L"· Generating primes to 800,000... ");

    tStart = std::chrono::high_resolution_clock::now();
    {
        std::vector<bool> isPrime(limit + 1, true);
        for (int p = 2; p * p <= limit; p++) {
            if (isPrime[p]) {
                for (int i = p * p; i <= limit; i += p)
                    isPrime[i] = false;
            }
        }
        for (int i = 2; i <= limit; i++) if (isPrime[i]) primeCount++;
    }
    tEnd = std::chrono::high_resolution_clock::now();
    tElapsed = tEnd - tStart;

    timeMsg.Format(L"Elapsed: %.0f ms. Count: %d", tElapsed.count(), primeCount);
    logger.Log(timeMsg);
    logger.Log(L"");
    ::PostMessage(hWnd, WM_APP_PROGRESS_UPDATE, (WPARAM)(100), 0);

    // --- Footer ---
    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDuration = totalEnd - totalStart;

    // Розрахунок годин, хвилин, секунд
    long long totalMs = std::chrono::duration_cast<std::chrono::milliseconds>(totalDuration).count();
    int h = (int)(totalMs / 3600000);
    int m = (int)((totalMs % 3600000) / 60000);
    int s = (int)((totalMs % 60000) / 1000);
    int ms = (int)(totalMs % 1000);

    double avgMips = 0;
    if (!allMipsResults.empty())
        avgMips = std::accumulate(allMipsResults.begin(), allMipsResults.end(), 0.0) / allMipsResults.size();

    logger.Log(L"*** EXECUTED TESTS AVERAGE: " + CString(std::to_wstring(avgMips).c_str()) + L" MIPS ***");
    logger.Log(L"");

    CString footerTime;
    footerTime.Format(L"*** ELAPSED TIME: %d Hour %d Min %d Sec %d Thousands ***", h, m, s, ms);
    logger.Log(footerTime);

    ::PostMessage(hWnd, WM_APP_BENCHMARK_FINISHED, 0, 0);
    return 0;
}

// Інші стандартні методи MFC (SysCommand, Paint, DragIcon) залишаються без змін...
void CSimpleCpuTestCppUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        // CDialogEx::OnSysCommand(nID, lParam); // Якщо є About box
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CSimpleCpuTestCppUIDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CSimpleCpuTestCppUIDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

void CSimpleCpuTestCppUIDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // Перевіряємо, чи створені елементи (OnSize викликається дуже рано, до створення кнопок)
    if (m_btnStart.GetSafeHwnd() == nullptr) return;

    // Відступи (можна налаштувати під себе)
    int margin = 10;       // Відступ від країв
    int gap = 10;          // Відступ між елементами
    int topBarHeight = 30; // Висота верхньої панелі (кнопка + прогрес)
    int btnWidth = 150;    // Ширина кнопки (фіксована)

    // 1. Кнопка (Start Benchmark)
    // Залишається зліва зверху: (x, y, width, height)
    m_btnStart.MoveWindow(margin, margin, btnWidth, topBarHeight);

    // 2. Прогрес-бар
    // Починається після кнопки і йде до кінця вікна
    int progressX = margin + btnWidth + gap;
    int progressWidth = cx - progressX - margin; // cx - це ширина клієнтської області вікна

    if (progressWidth > 0) // Щоб не було помилок при дуже сильному звуженні
    {
        m_progressBar.MoveWindow(progressX, margin, progressWidth, topBarHeight);
    }

    // 3. Едітор (Лог)
    // Починається під кнопкою і займає все інше місце
    int editY = margin + topBarHeight + gap;
    int editWidth = cx - (2 * margin);
    int editHeight = cy - editY - margin; // cy - це висота клієнтської області

    if (editHeight > 0 && editWidth > 0)
    {
        m_editLog.MoveWindow(margin, editY, editWidth, editHeight);
    }
}
