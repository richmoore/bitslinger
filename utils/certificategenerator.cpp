#include <QDebug>

#include "openssl_symbol_helper.h"

#include "certificategenerator.h"

QByteArray x509_to_der(X509 *x509);
QByteArray RSAPrivateKey_to_der(RSA *rsa);

CertificateGenerator::CertificateGenerator()
{
    osh_resolveOpenSslSymbols();
}

QSslCertificate CertificateGenerator::createClone(const QSslCertificate &leaf)
{
    X509 *leaf_x509 = (X509 *)(leaf.handle());
    X509 *ca_x509 = (X509 *)(m_caCert.handle());

    X509 *out = osh_X509_new();

    // Copy data
    long version = osh_X509_get_version(leaf_x509);
    osh_X509_set_version(out, version);

    ASN1_INTEGER *serial = osh_X509_get_serialNumber(leaf_x509);
    osh_X509_set_serialNumber(out, serial);

    X509_NAME *subject = osh_X509_get_subject_name(leaf_x509);
    osh_X509_set_subject_name(out, subject);

    ASN1_TIME *not_before = osh_X509_get_notBefore(leaf_x509);
    osh_X509_set_notBefore(out, not_before);

    ASN1_TIME *not_after = osh_X509_get_notAfter(leaf_x509);
    osh_X509_set_notAfter(out, not_after);

    // Only copy the SAN (avoids the keyids being wrong, name constraints etc.)
    int san_idx = osh_X509_get_ext_by_NID(leaf_x509, NID_subject_alt_name, 0);
    X509_EXTENSION *ext = osh_X509_get_ext(leaf_x509, san_idx);
    osh_X509_add_ext(out, ext, 0);

    // Stuff from CA
    X509_NAME *issuer = osh_X509_get_subject_name(ca_x509);
    osh_X509_set_issuer_name(out, issuer);

    // Use our leaf key
    RSA *rsa = (RSA *)(m_leafKey.handle());
    EVP_PKEY *pubkey = osh_EVP_PKEY_new();
    osh_EVP_PKEY_assign(pubkey, EVP_PKEY_RSA, (void *)(rsa));
    osh_X509_set_pubkey(out, pubkey);

    // Sign the new leaf with the CA key
    RSA *carsa = (RSA *)(m_caKey.handle());
    EVP_PKEY *capubkey = osh_EVP_PKEY_new();
    osh_EVP_PKEY_assign(capubkey, EVP_PKEY_RSA, (void *)(carsa));

    osh_X509_sign(out, capubkey, osh_EVP_sha256());

    QByteArray der = x509_to_der(out);
    return QSslCertificate(der, QSsl::Der);
}

QSslCertificate CertificateGenerator::createCaCertificate() const
{
    return QSslCertificate();
}

QSslKey CertificateGenerator::createKey() const
{
    BIGNUM *bne = osh_BN_new();
    RSA *rsa = osh_RSA_new();

    osh_BN_set_word(bne, RSA_F4);
    int result = osh_RSA_generate_key_ex( rsa, 2048, bne, NULL);
    if (!result) {
        qDebug() << "createKey() failed";
    }

    QByteArray der = RSAPrivateKey_to_der(rsa);
    qDebug() << der;
    QSslKey key(der, QSsl::Rsa, QSsl::Der);

    osh_RSA_free(rsa);
    osh_BN_free(bne);

    return key;
}

QByteArray x509_to_der(X509 *x509)
{
    // Use i2d_X509 to convert the X509 to an array.
    int length = osh_i2d_X509(x509, 0);
    QByteArray array;
    array.resize(length);
    char *data = array.data();
    char **dataP = &data;
    unsigned char **dataPu = (unsigned char **)dataP;
    if (osh_i2d_X509(x509, dataPu) < 0)
        return QByteArray();

    return array;
}

QByteArray RSAPrivateKey_to_der(RSA *rsa)
{
    // Use i2d_RSA to convert the RSA to an array.
    int length = osh_i2d_RSAPrivateKey(rsa, 0);
    QByteArray array;
    array.resize(length);
    char *data = array.data();
    char **dataP = &data;
    unsigned char **dataPu = (unsigned char **)dataP;
    if (osh_i2d_RSAPrivateKey(rsa, dataPu) < 0)
        return QByteArray();

    return array;
}
