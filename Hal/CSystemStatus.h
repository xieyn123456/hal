#pragma once
#ifndef __CSYSTEMSTATUS_H__
#define __CSYSTEMSTATUS_H__
#include<iostream>
#include <sstream>
#include <string>
#include <atlstr.h>
#include <vector>
#include <Shlwapi.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") 
#pragma comment(lib, "Pdh.lib")
#pragma comment(lib, "Shlwapi.lib")

typedef struct tagDISKSTATUS
{
	DWORD	_FreeAvalidToCaller;	//用于容纳调用者可用的字节数量 BYTE
	DWORD	_TotalNummber;			//用于容纳磁盘上的总字节数	BYTE
	DWORD	_TotalNummberOfFree;	//用于容纳磁盘上可用的字节数	BYTE
}DISKSTATUS, * LPDISKSTATUS;
//所有磁盘的状态
typedef struct tagAllDISKSTATUS
{
	UINT	_DiskCount;				//磁盘数量
	DWORD	_Total;					//所有磁盘总容量MB
	DWORD	_OfFree;				//所有磁盘剩余容量MB
}AllDISKSTATUS, * LPAllDISKSTATUS;

typedef struct tagEACHDISKSTATUS
{
	CString strdir;					//磁盘名称
	float	_Total;					//磁盘总容量MB
	float	_OfFree;				//磁盘剩余容量MB
}EACHDISKSTATUS, * LPEACHDISKSTATUS;

typedef struct tagNETCARDINFO
{
	CString Name;				//网卡名称
	CString Description;		//网卡描述
	CString Local_IP;			//IP地址
	CString Local_Mac;			//MAC地址
}NETCARDINFO, * LPNETCARDINFO;


#define SYSSTATE_NONE			0x00000000
#define SYSSTATE_CPU_USAGE		0x00000001
#define SYSSTATE_DISK_READ		0x00000002
#define SYSSTATE_DISK_WRITE		0x00000004
#define SYSSTATE_NET_DOWNLOAD	0x00000008
#define SYSSTATE_NET_UPLOAD		0x00000010

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion; // Binary driver version. 
	BYTE bRevision; // Binary driver revision. 
	BYTE bReserved; // Not used. 
	BYTE bIDEDeviceMap; // Bit map of IDE devices. 
	DWORD fCapabilities; // Bit mask of driver capabilities. 
	DWORD dwReserved[4]; // For future use. 
} GETVERSIONOUTPARAMS, * PGETVERSIONOUTPARAMS, * LPGETVERSIONOUTPARAMS;

//typedef struct _IDEREGS
//{
//	BYTE bFeaturesReg; // Used for specifying SMART "commands". 
//	BYTE bSectorCountReg; // IDE sector count register 
//	BYTE bSectorNumberReg; // IDE sector number register 
//	BYTE bCylLowReg; // IDE low order cylinder value 
//	BYTE bCylHighReg; // IDE high order cylinder value 
//	BYTE bDriveHeadReg; // IDE drive/head register 
//	BYTE bCommandReg; // Actual IDE command. 
//	BYTE bReserved; // reserved for future use. Must be zero. 
//} IDEREGS, * PIDEREGS, * LPIDEREGS;

//typedef struct _SENDCMDINPARAMS
//{
//	DWORD cBufferSize; // Buffer size in bytes 
//	IDEREGS irDriveRegs; // Structure with drive register values. 
//	BYTE bDriveNumber; // Physical drive number to send command to (0,1,2,3). 
//	BYTE bReserved[3]; // Reserved for future expansion. 
//	DWORD dwReserved[4]; // For future use. 
//} SENDCMDINPARAMS, * PSENDCMDINPARAMS, * LPSENDCMDINPARAMS;

//typedef struct _DRIVERSTATUS
//{
//	BYTE bDriverError; // Error code from driver, 
//	// or 0 if no error. 
//	BYTE bIDEStatus; // Contents of IDE Error register. 
//	// Only valid when bDriverError 
//	// is SMART_IDE_ERROR. 
//	BYTE bReserved[2]; // Reserved for future expansion. 
//	DWORD dwReserved[2]; // Reserved for future expansion. 
//} DRIVERSTATUS, * PDRIVERSTATUS, * LPDRIVERSTATUS;

//typedef struct _SENDCMDOUTPARAMS
//{
//	DWORD cBufferSize; // Size of bBuffer in bytes 
//	DRIVERSTATUS DriverStatus; // Driver status structure. 
//	BYTE bBuffer[512]; // Buffer of arbitrary length 
//	// in which to store the data read from the drive. 
//} SENDCMDOUTPARAMS, * PSENDCMDOUTPARAMS, * LPSENDCMDOUTPARAMS;

typedef struct _IDSECTOR
{
	USHORT wGenConfig;
	USHORT wNumCyls;
	USHORT wReserved;
	USHORT wNumHeads;
	USHORT wBytesPerTrack;
	USHORT wBytesPerSector;
	USHORT wSectorsPerTrack;
	USHORT wVendorUnique[3];
	CHAR sSerialNumber[20];
	USHORT wBufferType;
	USHORT wBufferSize;
	USHORT wECCSize;
	CHAR sFirmwareRev[8];
	CHAR sModelNumber[40];
	USHORT wMoreVendorUnique;
	USHORT wDoubleWordIO;
	USHORT wCapabilities;
	USHORT wReserved1;
	USHORT wPIOTiming;
	USHORT wDMATiming;
	USHORT wBS;
	USHORT wNumCurrentCyls;
	USHORT wNumCurrentHeads;
	USHORT wNumCurrentSectorsPerTrack;
	ULONG ulCurrentSectorCapacity;
	USHORT wMultSectorStuff;
	ULONG ulTotalAddressableSectors;
	USHORT wSingleWordDMA;
	USHORT wMultiWordDMA;
	BYTE bReserved[128];
} IDSECTOR, * PIDSECTOR;

#define DFP_GET_VERSION			0x00074080 
#define DFP_SEND_DRIVE_COMMAND	0x0007c084 
#define DFP_RECEIVE_DRIVE_DATA	0x0007c088 

#include "pdh.h"

class CSystemStatus
{
public:
	CSystemStatus();
	~CSystemStatus();
public:
	void		SystemInit(DWORD object = SYSSTATE_CPU_USAGE);							//系统初始化(初始化多个项目时使用或运算连接)
	void		SystemUnInit();															//释放资源
	double		GetSystemNetDownloadRate();												//获取网络下载速度
	double		GetSystemNetUploadRate();												//获取网络上传速度
	double		GetSystemDiskReadRate();												//获取当前磁盘读速率
	double		GetSystemDiskWriteRate();												//获取当前磁盘写速率
	double		GetSystemCpuCurrentUsage();												//系统CPU使用率

	void		GetSystemDiskStatus(std::vector<EACHDISKSTATUS>& vectorDisk);           //获取各个磁盘使用状态
	void		GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree);	//获取系统总得磁盘使用状态
	void		GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller);	//获取当前磁盘使用状态
	double		GetSystemCurrentDiskUsage();											//获取当前磁盘使用率

	BOOL		GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed);//获取物理内存状态
	double		GetTotalPhysicalMemory();												//获取可用内存大小
	double		GetTotalPhysicalMemoryFree();											//获取空闲内存
	double		GetTotalPhysicalMemoryUsed();											//获取已使用内存大小
	double		GetPhysicalMemoryUsage();												//获取内存使用率

	void		GetNetCardInfo(std::vector<NETCARDINFO>& vectorNetCard);				//获取网卡信息
	void		GetOsInfo(CString& osinfo);                                         //获取操作系统信息 
	void		GetCpuInfo(CString& CPUinfo);										//获取CPU硬件信息 	
	void		GetCPUid(CString& CPUid);											//获取CPUid

	BOOL		GetHDSerial(CString& HDSerial);										//获取硬盘物理序列号（需要管理员权限）

	CString GetPcName();//获取电脑名
	CString GetPcMac();//获取Mac
	CString GetHardDiskSerialNumber();//获取硬盘ID
	CString GetBaseBoardSerialNumber();//获取主板ID
	int GetTraceIP(CString& strIP, CString& m_StrIDC_EDIT1);//获取IP
	CString GetTraceMac(CString strip);	//用IP获取MAC
	CString GetSystemName();			//读取操作系统的名称
	bool IsInternetBar();				// 是否为网吧
	bool IsHaveKey(HKEY hKey, char* lpSubKey);

	bool m_bIsInternetBar;

private:
	PDH_HQUERY		m_Query;
	PDH_HCOUNTER	m_CpuTotal, m_DiskRead, m_DiskWrite, m_NetDownload, m_NetUpload;

public:
	/*
	参数:const char *cmd
	systeminfo:查看详细的系统信息
	wmic logicaldisk:查看盘符
	fsutil volume diskfree + 盘符名称:查看某个盘符的容量大小。
	wmic path win32_physicalmedia get SerialNumber;查看硬盘系列号
	wmic diskdrive get serialnumber;查看硬盘系列号(和上面效果一样)
	wmic cpu:查看CPU运行信息
	wmic cpu list brief:查看CPU硬件信息
	wmic memorychip;查看系统内存信息
	wmic bios:查看系统的bios信息
	wmic memorychip list brief:查看内存条数
	wmic memcache list brief:查看缓存内存
	wmic diskdrive:查看磁盘详细信息
	wmic diskdrive get Name, Model:查看硬盘名称，型号（使用get）
	...
	*/
	CString execCmd(const char* cmd)
	{
		char buffer[128] = { 0 };
		CString result;
		FILE* pipe = _popen(cmd, "r");
		if (!pipe) throw std::runtime_error("_popen() failed!");
		while (!feof(pipe))
		{
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}
		_pclose(pipe);
		return result;
	}

	_inline void ChangeByteOrder(PCHAR szString, USHORT uscStrSize)
	{
		USHORT  i = 0;
		CHAR	temp = '\0';

		for (i = 0; i < uscStrSize; i += 2)
		{
			temp = szString[i];
			szString[i] = szString[i + 1];
			szString[i + 1] = temp;
		}
	}

};
#endif

