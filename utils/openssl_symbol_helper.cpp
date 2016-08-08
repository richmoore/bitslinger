#include <QSslSocket>
#include "openssl_symbol_helper.h"


#ifndef QT_LINKED_OPENSSL

namespace {
void opensslSymbolHelperUnresolvedSymbolWarning(const char *functionName)
{
    qWarning("opensslSymbolHelper: cannot call unresolved function %s", functionName);
}

void opensslSymbolHelperCannotResolveSymbolWarning(const char *functionName)
{
    qWarning("opensslSymbolHelper: cannot resolve %s", functionName);
}

}

#define RESOLVEFUNC(func) \
    if (!(_osh_##func = _osh_PTR_##func(libcrypto->resolve(#func)))) \
        opensslSymbolHelperCannotResolveSymbolWarning(#func);

#endif // QT_LINKED_OPENSSL


DEFINEFUNC(X509 *, X509_new, void, DUMMYARG, return 0, return)
DEFINEFUNC(void, X509_free, X509 *x, x, return, return)

DEFINEFUNC2(int, i2d_X509, X509 *a, a, unsigned char **b, b, return -1, return)

DEFINEFUNC(long, X509_get_version, X509 *x, x, return -1, return)
DEFINEFUNC2(int, X509_set_version, X509 *x, x, long version, version, return 0, return)

DEFINEFUNC(ASN1_INTEGER *, X509_get_serialNumber, X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_serialNumber, X509 *x, x, ASN1_INTEGER *serial, serial, return 0, return)

DEFINEFUNC(X509_NAME *, X509_get_issuer_name, X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_issuer_name, X509 *x, x, X509_NAME *name, name, return 0, return)

DEFINEFUNC(X509_NAME *, X509_get_subject_name, X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_subject_name, X509 *x, x, X509_NAME *name, name, return 0, return)

//DEFINEFUNC(ASN1_TIME *, X509_get_notBefore, const X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_notBefore, X509 *x, x, const ASN1_TIME *time, time, return 0, return)

//DEFINEFUNC(ASN1_TIME *, X509_get_notAfter, const X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_notAfter, X509 *x, x, const ASN1_TIME *time, time, return 0, return)

DEFINEFUNC(EVP_PKEY *, X509_get_pubkey, X509 *x, x, return 0, return)
DEFINEFUNC2(int, X509_set_pubkey, X509 *x, x, EVP_PKEY *pkey, pkey, return 0, return)

DEFINEFUNC3(int, X509_get_ext_by_NID, X509 *x, x, int nid, nid, int lastpos, lastpos, return 0, return)
DEFINEFUNC2(X509_EXTENSION *, X509_get_ext, X509 *x, x, int loc, loc, return 0, return)
DEFINEFUNC3(int, X509_add_ext, X509 *x, x, X509_EXTENSION *ex, ex, int loc, loc, return 0, return)

DEFINEFUNC3(int, X509_sign, X509 *x, x, EVP_PKEY *pkey, pkey, const EVP_MD *md, md, return 0, return)

DEFINEFUNC(const EVP_MD *, EVP_sha256, void, DUMMYARG, return 0, return)
DEFINEFUNC(EVP_PKEY *, EVP_PKEY_new, void, DUMMYARG, return 0, return)
DEFINEFUNC3(int, EVP_PKEY_assign, EVP_PKEY *pkey, pkey, int type, type, void *key, key, return 0, return)

#if !defined QT_LINKED_OPENSSL

bool osh_resolveOpenSslSymbols()
{
    static bool symbolsResolved = false;
    static bool triedToResolveSymbols = false;

    if (symbolsResolved)
        return true;
    if (triedToResolveSymbols)
        return false;
    triedToResolveSymbols = true;

    // Force openssl to be loaded by Qt
    bool supported = QSslSocket::supportsSsl();
    Q_UNUSED(supported);

    QLibrary *libcrypto = new QLibrary("libcrypto");
    if (!libcrypto->load()) {
        delete libcrypto;
        return false;
    }

    RESOLVEFUNC(X509_new)
    RESOLVEFUNC(X509_free)
    RESOLVEFUNC(i2d_X509)

    RESOLVEFUNC(X509_get_version)
    RESOLVEFUNC(X509_set_version)
    RESOLVEFUNC(X509_get_serialNumber)
    RESOLVEFUNC(X509_set_serialNumber)
    RESOLVEFUNC(X509_get_issuer_name)
    RESOLVEFUNC(X509_set_issuer_name)
    RESOLVEFUNC(X509_get_subject_name)
    RESOLVEFUNC(X509_set_subject_name)
//    RESOLVEFUNC(X509_get_notBefore)
    RESOLVEFUNC(X509_set_notBefore)
//    RESOLVEFUNC(X509_get_notAfter)
    RESOLVEFUNC(X509_set_notAfter)
    RESOLVEFUNC(X509_get_pubkey)
    RESOLVEFUNC(X509_set_pubkey)

    RESOLVEFUNC(X509_get_ext_by_NID)
    RESOLVEFUNC(X509_get_ext)
    RESOLVEFUNC(X509_add_ext)

    RESOLVEFUNC(X509_sign)

    RESOLVEFUNC(EVP_sha256)
    RESOLVEFUNC(EVP_PKEY_new)
    RESOLVEFUNC(EVP_PKEY_assign)

    symbolsResolved = true;
    delete libcrypto;
    return true;
}

#else
bool osh_resolveOpenSslSymbols()
{
#ifdef QT_NO_OPENSSL
    return false;
#endif
    return true;
}
#endif // !defined QT_LINKED_OPENSSL
