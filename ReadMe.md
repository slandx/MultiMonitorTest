#
[Source Page](https://www.codeproject.com/Articles/3690/MFC-classes-for-multiple-monitors)

## Background
Eveyrything you ever wanted to know about the API for multiple monitors is described in a very well written [article by David Campbell](http://www.microsoft.com/msj/0697/monitor/monitor.htm) in the June 1997 issue of MSJ.

The API itself is simple and straightforward. It has some new constants to pass to `GetSystemMetrics` and a couple methods to enumerate all of the monitors currently attached to the system and get the properties of a given monitor.

All of the types and functions are defined by the platform SDK in the file `multimon.h` . If you `#define WINVER` to be greater than `0x400` you don't need to link to `multimon.h`, as the API gets defined in windef.h for Windows 98 and later targeted builds. `Multimon.h` also provides some stub functions that allow calls to be safely made on Windows 95/NT4 machines. These wrappers interrogate the runtime OS and either make fall-through calls into the actual API or return the propeties of the one (and only) monitor on those older platforms.

The multi monitor classes take care of including `multimon.h` correctly depending on the value of `WINVER`.

## Using the code
`CMointor` is a basic MFC class that allows you to safely use the multi-monitor API on any Win32 platform.

There are three classes in this library:

`CMonitors` represents the collection of monitors currently attached to the system and wraps the `EnumDisplayMonitors` API function.

```c++
//CMonitors' interface
CMonitor GetMonitor( const int index ) const;
int GetCount() const;

//returns the monitor closest to the specified item
static CMonitor GetNearestMonitor( const LPRECT lprc );
static CMonitor GetNearestMonitor( const POINT pt );
static CMonitor GetNearestMonitor( const CWnd* pWnd );

//is the specificed item visible on any monitor
static BOOL IsOnScreen( const POINT pt );
static BOOL IsOnScreen( const CWnd* pWnd );
static BOOL IsOnScreen( const LPRECT lprc );

//returns the rectangle encompassing all monitors
static void GetVirtualDesktopRect( LPRECT lprc );

//determines whether the given handle is a valid monitor handle
static BOOL IsMonitor( const HMONITOR hMonitor );
static CMonitor GetPrimaryMonitor();
static BOOL AllMonitorsShareDisplayFormat();

static int GetMonitorCount();
```

`CMonitor` is a wrapper around an `HMONITOR` handle (returned from `EnumDisplayMonitors`) and the `GetMonitorInfo` function. With `CMonitor` you can get at the characteristics of a given monitor.

```c++
//The interface of CMonitor
void Attach( const HMONITOR hMonitor );
HMONITOR Detach();
void ClipRectToMonitor( LPRECT lprc,
                        const BOOL UseWorkAreaRect = FALSE ) const;
void CenterRectToMonitor( LPRECT lprc,
                        const BOOL UseWorkAreaRect = FALSE ) const;
void CenterWindowToMonitor( CWnd* const pWnd,<BR> const BOOL UseWorkAreaRect = FALSE ) const;

//creates a device context for the monitor - the client is responsible for
// DeleteDC HDC CreateDC() const;
void GetMonitorRect( LPRECT lprc ) const;

//the work area is the monitor rect minus the start bar
void GetWorkAreaRect( LPRECT lprc ) const;
void GetName( CString& string ) const;
int GetBitsPerPixel() const;

//determines if the specified item on the monitor
BOOL IsOnMonitor( const POINT pt ) const;
BOOL IsOnMonitor( const CWnd* pWnd ) const;
BOOL IsOnMonitor( const LPRECT lprc ) const;
BOOL IsPrimaryMonitor() const;
BOOL IsMonitor() const;
```

`CMonitorDC` is a `CDC` derived class that represents a monitor specific device context. I haven't really gone to far with this class but it seemed like a logical part of the library.

## Known Limitations
`CMonitor` and `CMonitors` rely on the assumption that a monitor handle does not change. This has proved to be a safe assumption empirically but isn't nessecarily a guarantee.

## History
* 02/20/2003 - Initial Release
* 08/25/2003 - Made changes to make compatible with VC6 environment

## License
This article, along with any associated source code and files, is licensed under [The Code Project Open License (CPOL)](http://www.codeproject.com/info/cpol10.aspx).
