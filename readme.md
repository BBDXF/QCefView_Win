https://tishion.github.io/QCefView 
# 原版存在一些问题，也没有太多解释，这个fork主要解决两个问题：

# 1. `onQCefUrlRequest`接口无效问题 
`void onQCefUrlRequest(const QString& url);`
问题的关键解决点在 `QCefViewDefaultSchemeHandler.cpp` 100行左右的位置，如下：
```cpp
// handler factory
CefRefPtr<CefResourceHandler> SchemeHandlerFactory::Create(
	CefRefPtr<CefBrowser> browser, 
	CefRefPtr<CefFrame> frame, 
	const CefString& scheme_name, 
	CefRefPtr<CefRequest> request)
{
	// 
	// TO DO (Get the correct SchemeHandler corresponding to the browser)
	// 
	return new SchemeHandler(nullptr); // 这里需要QCefView指针，默认提供了nullptr
}
```

# 2. 核心类分析
`QCefView`如何控制Cef的呢？  
`QCefView`类真正起作用的是`Implementation`类,它内部包含两个主要的类：
1. `QPointer<CCefWindow> pCefWindow_;` 用来产生Cef窗口相关，依次通过如下方式包裹
```cpp
/** Outline of QCefView:
 **
 **		+--------------------------------------------------------------+
 **		| QCefView(QWidget)                                            |
 **		|  						                                       |
 **		|    +----------------------------------------------------+    |
 **		|    | WindowWrapper(QWidget)                             |    |
 **		|    |                                                    |    |
 **		|    |    +-------------------------------------------+   |    |
 **		|    |	  | CefWindow(QWindow)                        |   |    |
 **		|    |	  |					                          |   |    |
 **		|    |	  |  	-- This is Cef Navi UI Surface  --    |   |    |
 **		|    |	  |					                          |   |    |
 **		|    |	  +-------------------------------------------+   |    |
 **		|    |                                                    |    |
 **		|    +----------------------------------------------------+    |
 **		|                                                              |
 **		+--------------------------------------------------------------+
 **
 **	Remarks:
 **		The WindowWrapper and CefWindow are transparent to upper layer user.
 **
 **/
```
2. `CefRefPtr<QCefViewBrowserHandler> pQCefViewHandler_;` 它实现了很多Cef原生的接口，用来接收Cef的事件等。但是，需要注意的是，并不是所有的功能都在这里实现的。  
3. 在项目中有一个最重要的类`CCefWindow`，它在`Implementation`构造时创建，并且内部有一个隐藏的`CCefManager`类，它才是真正操纵者。注意`CCefWindow.cpp`文件中的构造函数，如下：  
```cpp
class QCefView::Implementation
{
public:
	explicit Implementation(const QString& url, QWindow* parent)
		: pCefWindow_(nullptr), pQCefViewHandler_(nullptr)
	{
		// Here we must create a QWidget as a wrapper to encapsulate the QWindow
		pCefWindow_ = new CCefWindow(parent);
		pCefWindow_->create();

		// Set window info
		CefWindowInfo window_info;
		RECT rc = { 0 };
		window_info.SetAsChild((HWND)pCefWindow_->winId(), rc);

		// 后续省略...
	}
};
		
CCefWindow::CCefWindow(QWindow *parent /*= 0*/)
	: QWindow(parent)
	, hwndCefBrowser_(nullptr)
{
	setFlags(Qt::FramelessWindowHint);

	CCefManager::getInstance().initializeCef();
}
```

`CCefManager::getInstance().initializeCef();`才是对CEF真正的初始化者。  

# 其他
编译方式同原版。
工具：Qt5+VS2015

# 启用Flash
前提：先安装好 Flash的Chrome版本。 
```cpp
// QCefViewBrowserApp.cpp line 24
void QCefViewBrowserApp::OnBeforeCommandLineProcessing(
	const CefString& process_type, 
	CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitch("disable-spell-checking");
	//command_line->AppendSwitch("disable-extensions");
	command_line->AppendSwitch("disable-pdf-extension");
	command_line->AppendSwitch("enable-direct-write");
	command_line->AppendSwitch("allow-file-access-from-files");
	command_line->AppendSwitchWithValue("renderer-process-limit", "1");
	command_line->AppendSwitch("--disable-web-security");//关闭同源策略
	command_line->AppendSwitch("--enable-system-flash");//使用系统flash
}

// QCefView.cpp line 39
browserSettings.plugins = STATE_DEFAULT;	// disable all plugins
```

