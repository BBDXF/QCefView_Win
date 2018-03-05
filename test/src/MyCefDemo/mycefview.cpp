#include "mycefview.h"
#include <QMessageBox>
#include "Windows.h"

MyCefView::MyCefView(const QString url, QWidget *parent):
    QCefView(url, parent)
{

}

void MyCefView::onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward)
{
    qDebug()<<"isLoading:"<<isLoading<<"canGoBack"<<canGoBack<<"canGoForward"<<canGoForward;
}

void MyCefView::onLoadStart()
{
    qDebug()<<"onLoadStart";
}

void MyCefView::onLoadEnd(int httpStatusCode)
{
    qDebug()<<"onLoadEnd"<<httpStatusCode;
}

void MyCefView::onLoadError(int errorCode, const QString &errorMsg, const QString &failedUrl)
{
    qDebug()<<"onLoadError"<<errorCode<<errorMsg<<failedUrl;
}

void MyCefView::onQCefUrlRequest(const QString &url)
{
    qDebug()<<"onQCefUrlRequest"<<url;
}

void MyCefView::onQCefQueryRequest(const QCefQuery &query)
{
    qDebug()<<"onQCefQueryRequest"<<query.id()<<query.reqeust();

    QString text = QString(
        "Current Msg From: mycefview main thread\r\n"
        "Query: %1").arg(query.reqeust());

    //QMessageBox::information(this->window(), title, text);

    //QString response = query.reqeust().toUpper();
    query.setResponseResult(true, text);
    responseQCefQuery(query);
}

void MyCefView::onInvokeMethodNotify(int browserId, int frameId, const QString &method, const QVariantList &arguments)
{
    qDebug()<<"onInvokeMethodNotify"<<browserId<<frameId<<method<<arguments;

    if (0 == method.compare("onDragAreaMouseDown"))
    {
        HWND hWnd = ::GetAncestor((HWND)getCefWinId(), GA_ROOT);

        // get current mouse cursor position
        POINT pt;
        ::GetCursorPos(&pt);

        // in case the mouse is being captured, try to release it
        ::ReleaseCapture();

        // simulate that the mouse left button is down on the title area
        ::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, POINTTOPOINTS(pt));
        return;
    }

    QString title("QCef InvokeMethod Notify");
    QString text = QString(
        "Current Thread: mycefview main thread\r\n"
        "Method: %1\r\n"
        "Arguments: ...")
        .arg(method);

    QMessageBox::information(this->window(), title, text);
}
