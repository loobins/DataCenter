#pragma once
#include <mutex>
#include <atomic>
#include <string>
#include <forward_list>
#include <concurrentqueue.h>
#include "basewindow.h"
#include "stockdrv.h"
#include "worker.h"


constexpr auto KB = 1024;
constexpr auto PARALLEL = 128;
constexpr auto WM_RECV_DATA = WM_USER + 1;

// Function signature of StockDrv
typedef int (WINAPI *fnStockInit)(HWND hWnd, UINT Msg, int nWorkMode);
typedef int (WINAPI *fnStockQuit)(HWND hWnd);;
typedef DWORD(WINAPI* fnGetStockDrvInfo)(int nInfo, void* pBuf);


class MainWindow : public BaseWindow<MainWindow>
{
	enum class State {
		Start,
		StartNeZip,
		StartDrv,
		ReceiveData,
		Error
	} m_state;

	std::wstring m_wcsError;
	std::wstring m_wcsNeZipTitle;
	WCHAR m_szNeZipPath[MAX_PATH];
	HWND m_hWndNeZip;

	BufferQueue_t m_bufferQueue;
	size_t m_nTotalReceived;
	size_t m_nTotalSent;

	std::string m_uri;
	nng_socket m_sock;
	std::forward_list<Worker> m_workers;
	std::promise<void> m_pExit;

protected:
	void StateMachine();
	void StartDrv();
	void StartNeZip();
	void PaintWindow(HDC& hdc, RECT& rect);
	void KillTimer();
	bool LoadDll();
	void MoveWindow();

	void OnRecvData(WPARAM wParam, LPARAM lParam);
	void OnRecvReport(RCV_DATA *pRcvData);
	void OnRecvFileData(RCV_DATA *pRcvData);

	HINSTANCE m_hStockDll;
	fnStockInit Stock_Init;
	fnStockQuit Stock_Quit;
	fnGetStockDrvInfo GetStockDrvInfo;

	void Fatal(const WCHAR *message, int rv);
public:
	PCWSTR  ClassName() const { return L"NeZipReceiver Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK CheckNeZipStarted(HWND hWnd, LPARAM lParam);

	void LoadConfig(LPCWSTR szIniFilePath);
	void StartWorkers();
	void WaitWorkers();

public:
	MainWindow();
	MainWindow(const MainWindow&) = delete;
	MainWindow(MainWindow&&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	MainWindow& operator=(MainWindow&&) = delete;
	~MainWindow() override;
};
