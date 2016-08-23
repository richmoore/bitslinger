#include <QTcpSocket>
#include <QDebug>

#include "httpproxyrequesthandler.h"

HttpProxyRequestHandler::HttpProxyRequestHandler(QObject *parent)
    : QObject(parent),
    m_state(NotReadyState),
    m_sock(0)
{
}

void HttpProxyRequestHandler::setSocket(QTcpSocket *sock)
{
    m_sock = sock;
    m_state = ReadingFirstLineState;

    connect(m_sock, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

void HttpProxyRequestHandler::dataReceived()
{
    while (m_sock->canReadLine()) {
        QByteArray line = m_sock->readLine();

        // 0123456789abcdef0123456789abcdef
        // CONNECT example.com:443 HTTP/1.0
        if (m_state == ReadingFirstLineState) {
            int firstSpace = line.indexOf(' '); // 7
            int secondSpace = line.indexOf(' ', firstSpace+1); // 17

            qDebug() << "first space" << firstSpace << "second space" << secondSpace;

            if (firstSpace < 1 || secondSpace < firstSpace+2) {
                qDebug() << "Bad request" << line;
                m_state = ErrorState;
                emit invalidRequest();
                //////// BADNESS
            }

            QByteArray verb = line.mid(0, firstSpace);
            QByteArray hostPort = line.mid(firstSpace+1, secondSpace-(firstSpace+1));
            m_httpVersion = line.mid(secondSpace+1, line.length()-(secondSpace+1)).trimmed();

            qDebug() << "Verb" << verb;
            qDebug() << "hostport" << hostPort;
            qDebug() << "httpVer" << m_httpVersion;

            int colon = hostPort.indexOf(':');
            if (colon < 1 || colon == hostPort.length()) {
                m_state = ErrorState;
                emit invalidRequest();
                //////// BADNESS
            }

            m_host = QString::fromLatin1(hostPort.left(colon));
            m_port = hostPort.mid(colon+1).toInt();

            m_state = ReadingHeadersState;
        }
        else if (m_state == ReadingHeadersState) {
            if (line == QByteArrayLiteral("\n") || line == QByteArrayLiteral("\r\n") ) {
                qDebug() << "End of headers";
                m_state = RequestReadyState;
                emit requestReady();
                return;
            }
            else {
                qDebug() << "Proxy Header" << line;
            }
        }
    }
}

void HttpProxyRequestHandler::connectionSucceeded()
{
    Q_ASSERT(m_state == RequestReadyState);

    m_sock->write(m_httpVersion);
    m_sock->write(QByteArrayLiteral(" 200 OK\r\n\r\n"));
    m_state = CompletedState;
}

void HttpProxyRequestHandler::connectionFailed()
{
    Q_ASSERT(m_state == RequestReadyState);

    m_sock->write(m_httpVersion);
    m_sock->write(QByteArrayLiteral(" 503 FAILED\r\n\r\n"));
    m_state = ErrorState;
}
