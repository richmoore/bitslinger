#ifndef HTTPPROXYREQUESTHANDLER_H
#define HTTPPROXYREQUESTHANDLER_H

#include <QObject>

class QTcpSocket;

class HttpProxyRequestHandler : public QObject
{
    Q_OBJECT
public:
    enum State {
        NotReadyState,
        ReadingFirstLineState,
        ReadingHeadersState,
        RequestReadyState,
        CompletedState,
        ErrorState
    };

    explicit HttpProxyRequestHandler(QObject *parent = 0);

    void setSocket(QTcpSocket *sock);

    QString host() const { return m_host; }
    int port() const { return m_port; }

public slots:
    void connectionSucceeded();
    void connectionFailed();

signals:
    void requestReady();
    void invalidRequest();

protected slots:
    void dataReceived();
    void sendBadRequest();
    void sendBadMethod();

private:
    State m_state;
    QTcpSocket *m_sock;
    QString m_host;
    int m_port;
    QByteArray m_httpVersion;
};

#endif // HTTPPROXYREQUESTHANDLER_H
