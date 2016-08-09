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

// X509
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

DEFINEFUNC2(ASN1_TIME *, X509_gmtime_adj, ASN1_TIME *s, s, long adj, adj, return 0, return)

DEFINEFUNC7(int, X509_NAME_add_entry_by_txt,
            X509_NAME *name, name,
            const char *field, field,
            int type, type,
            const unsigned char *bytes, bytes,
            int len, len,
            int loc, loc,
            int set, set,
            return 0, return)


// EVP
DEFINEFUNC(const EVP_MD *, EVP_sha256, void, DUMMYARG, return 0, return)
DEFINEFUNC(EVP_PKEY *, EVP_PKEY_new, void, DUMMYARG, return 0, return)
DEFINEFUNC3(int, EVP_PKEY_assign, EVP_PKEY *pkey, pkey, int type, type, void *key, key, return 0, return)

// BIGNUM
DEFINEFUNC(BIGNUM *, BN_new, void, DUMMYARG, return 0, return)
DEFINEFUNC(void, BN_free, BIGNUM *a, a, return, return)

DEFINEFUNC2(int, BN_set_word, BIGNUM *a, a, BN_ULONG w, w, return 0, return)

// RSA
DEFINEFUNC(RSA *, RSA_new, void, DUMMYARG, return 0, return)
DEFINEFUNC(void, RSA_free, RSA *rsa, rsa, return, return)

DEFINEFUNC2(int, i2d_RSAPrivateKey, RSA *rsa, rsa, unsigned char **b, b, return 0, return)

DEFINEFUNC4(int, RSA_generate_key_ex, RSA *rsa, rsa, int bits, bits, BIGNUM *e, e, BN_GENCB *cb, cb, return 0, return)

// ASN1
DEFINEFUNC(ASN1_INTEGER *, ASN1_INTEGER_new, void, DUMMYARG, return 0, return)
DEFINEFUNC2(int, ASN1_INTEGER_set, ASN1_INTEGER *a, a, long v, v, return 0, return)

// Extensions
DEFINEFUNC(BASIC_CONSTRAINTS *, BASIC_CONSTRAINTS_new, void, DUMMYARG, return 0, return)
DEFINEFUNC3(X509_EXTENSION *, X509V3_EXT_i2d, int ext_nid, ext_nid, int crit, crit, void *ext_struc, ext_struc, return 0, return)


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
    if (!supported)
        return false;

#ifdef QT_OS_WIN
    QLibrary *libcrypto = new QLibrary("libeay32");
#else
    QLibrary *libcrypto = new QLibrary("libcrypto");
#endif
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

    RESOLVEFUNC(X509_gmtime_adj)
    RESOLVEFUNC(X509_NAME_add_entry_by_txt)

    RESOLVEFUNC(EVP_sha256)
    RESOLVEFUNC(EVP_PKEY_new)
    RESOLVEFUNC(EVP_PKEY_assign)

    RESOLVEFUNC(BN_new)
    RESOLVEFUNC(BN_free)

    RESOLVEFUNC(BN_set_word)

    RESOLVEFUNC(RSA_new)
    RESOLVEFUNC(RSA_free)
    RESOLVEFUNC(i2d_RSAPrivateKey)
    RESOLVEFUNC(RSA_generate_key_ex)

    RESOLVEFUNC(ASN1_INTEGER_new)
    RESOLVEFUNC(ASN1_INTEGER_set)
    RESOLVEFUNC(BASIC_CONSTRAINTS_new)
    RESOLVEFUNC(X509V3_EXT_i2d)

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
