#ifndef MYCEFVIEW_H
#define MYCEFVIEW_H

#include <QtCore>
#include <QtGui>
#include "QCefViewSDK/include/QCefView.h"


class MyCefView : public QCefView
{
public:
    MyCefView(const QString url, QWidget* parent = 0);

    // QCefView interface
protected slots:
    void onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward) override;
    void onLoadStart() override;
    void onLoadEnd(int httpStatusCode) override;
    void onLoadError(int errorCode, const QString &errorMsg, const QString &failedUrl) override;
    void onQCefUrlRequest(const QString &url) override;
    void onQCefQueryRequest(const QCefQuery &query) override;
    void onInvokeMethodNotify(int browserId, int frameId, const QString &method, const QVariantList &arguments) override;
};

#endif // MYCEFVIEW_H
