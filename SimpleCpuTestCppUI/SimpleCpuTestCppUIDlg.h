// SimpleCpuTestCppUIDlg.h : header file
#pragma once
#include "afxwin.h"

// Визначаємо власні повідомлення для оновлення UI з іншого потоку
#define WM_APP_LOG_UPDATE (WM_APP + 1)
#define WM_APP_PROGRESS_UPDATE (WM_APP + 2)
#define WM_APP_BENCHMARK_FINISHED (WM_APP + 3)

class CSimpleCpuTestCppUIDlg : public CDialogEx
{
    // Construction
public:
    CSimpleCpuTestCppUIDlg(CWnd* pParent = nullptr);

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SIMPLECPUTESTCPPUI_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;
    CBrush m_brushBack; // Для чорного фону
    CFont m_fontLog;    // Для моноширинного шрифту

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // Наші обробники
    afx_msg void OnBnClickedBtnStart();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // Для кольорів

    // Обробники повідомлень від потоку
    afx_msg LRESULT OnLogUpdate(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnProgressUpdate(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnBenchmarkFinished(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

public:
    CButton m_btnStart;
    CProgressCtrl m_progressBar;
    CEdit m_editLog;

    // Статична функція потоку
    static UINT __cdecl BenchmarkThread(LPVOID pParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};