#ifndef CERTIFICATEGENERATOR_H
#define CERTIFICATEGENERATOR_H

#include <QSslCertificate>
#include <QSslKey>

class CertificateGenerator
{
public:
    CertificateGenerator();

    QSslCertificate caCertificate() const { return m_caCert; }
    void setCaCertificate(const QSslCertificate &caCert) { m_caCert = caCert; }

    QSslKey caKey() const { return m_caKey; }
    void setCaKey(const QSslKey &caKey) { m_caKey = caKey; }

    QSslKey leafKey() const { return m_leafKey; }
    void setLeafKey(const QSslKey &leafKey) { m_leafKey = leafKey; }

    QSslCertificate createClone(const QSslCertificate &leaf);

    QSslCertificate createCaCertificate(const QSslKey &caKey) const;
    QSslKey createKey() const;

private:
    QSslCertificate m_caCert;
    QSslKey m_caKey;
    QSslKey m_leafKey;
};

#endif // CERTIFICATEGENERATOR_H
