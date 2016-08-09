#ifndef OPENSSL_SYMBOL_HELPER_H
#define OPENSSL_SYMBOL_HELPER_H

#include <QLibrary>
#include <qglobal.h>

#include "openssl/crypto.h"
#include "openssl/rsa.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"

#define DUMMYARG

#if !defined QT_LINKED_OPENSSL

#  define DEFINEFUNC(ret, func, arg, a, err, funcret) \
    typedef ret (*_osh_PTR_##func)(arg); \
    static _osh_PTR_##func _osh_##func = 0; \
    ret osh_##func(arg) { \
        if (Q_UNLIKELY(!_osh_##func)) { \
            opensslSymbolHelperUnresolvedSymbolWarning(#func); \
            err; \
        } \
        funcret _osh_##func(a); \
    }

// ret func(arg1, arg2)
#  define DEFINEFUNC2(ret, func, arg1, a, arg2, b, err, funcret) \
    typedef ret (*_osh_PTR_##func)(arg1, arg2);         \
    static _osh_PTR_##func _osh_##func = 0;               \
    ret osh_##func(arg1, arg2) { \
        if (Q_UNLIKELY(!_osh_##func)) { \
            opensslSymbolHelperUnresolvedSymbolWarning(#func);\
            err; \
        } \
        funcret _osh_##func(a, b); \
    }

// ret func(arg1, arg2, arg3)
#  define DEFINEFUNC3(ret, func, arg1, a, arg2, b, arg3, c, err, funcret) \
    typedef ret (*_osh_PTR_##func)(arg1, arg2, arg3);            \
    static _osh_PTR_##func _osh_##func = 0;                        \
    ret osh_##func(arg1, arg2, arg3) { \
        if (Q_UNLIKELY(!_osh_##func)) { \
            opensslSymbolHelperUnresolvedSymbolWarning(#func); \
            err; \
        } \
        funcret _osh_##func(a, b, c); \
    }

// ret func(arg1, arg2, arg3, arg4)
#  define DEFINEFUNC4(ret, func, arg1, a, arg2, b, arg3, c, arg4, d, err, funcret) \
    typedef ret (*_osh_PTR_##func)(arg1, arg2, arg3, arg4);               \
    static _osh_PTR_##func _osh_##func = 0;                                 \
    ret osh_##func(arg1, arg2, arg3, arg4) { \
         if (Q_UNLIKELY(!_osh_##func)) { \
             opensslSymbolHelperUnresolvedSymbolWarning(#func); \
             err; \
         } \
         funcret _osh_##func(a, b, c, d); \
    }

// ret func(arg1, arg2, arg3, arg4, arg6, arg7)
#  define DEFINEFUNC7(ret, func, arg1, a, arg2, b, arg3, c, arg4, d, arg5, e, arg6, f, arg7, g, err, funcret) \
    typedef ret (*_osh_PTR_##func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);   \
    static _osh_PTR_##func _osh_##func = 0;                                       \
    ret osh_##func(arg1, arg2, arg3, arg4, arg5, arg6, arg7) { \
        if (Q_UNLIKELY(!_osh_##func)) { \
            opensslSymbolHelperUnresolvedSymbolWarning(#func); \
            err; \
        } \
        funcret _osh_##func(a, b, c, d, e, f, g); \
    }

#else

#  define DEFINEFUNC(ret, func, arg, a, err, funcret) \
    ret osh_##func(arg) { funcret func(a); }

// ret func(arg1, arg2)
#  define DEFINEFUNC2(ret, func, arg1, a, arg2, b, err, funcret) \
    ret osh_##func(arg1, arg2) { funcret func(a, b); }

// ret func(arg1, arg2, arg3)
#  define DEFINEFUNC3(ret, func, arg1, a, arg2, b, arg3, c, err, funcret) \
    ret osh_##func(arg1, arg2, arg3) { funcret func(a, b, c); }

// ret func(arg1, arg2, arg3, arg4)
#  define DEFINEFUNC4(ret, func, arg1, a, arg2, b, arg3, c, arg4, d, err, funcret) \
    ret osh_##func(arg1, arg2, arg3, arg4) { funcret func(a, b, c, d); }

// ret func(arg1, arg2, arg3, arg4, arg6, arg7)
#  define DEFINEFUNC7(ret, func, arg1, a, arg2, b, arg3, c, arg4, d, arg5, e, arg6, f, arg7, g, err, funcret) \
    ret osh_##func(arg1, arg2, arg3, arg4, arg5, arg6, arg7) { funcret func(a, b, c, d, e, f, g); }

#endif // !defined QT_LINKED_OPENSSL

// Call this first!!
bool osh_resolveOpenSslSymbols();

//
// The forwarding functions
//

// X509

# define         osh_X509_get_version(x) osh_ASN1_INTEGER_get((x)->cert_info->version)
# define         osh_X509_get_notBefore(x) ((x)->cert_info->validity->notBefore)
# define         osh_X509_get_notAfter(x) ((x)->cert_info->validity->notAfter)

X509 *osh_X509_new();
void osh_X509_free(X509 *x);

// X509
int osh_i2d_X509(X509 *a, unsigned char **b);

//long osh_X509_get_version(X509 *x);
int osh_X509_set_version(X509 *x, long version);

int osh_X509_set_serialNumber(X509 *x, ASN1_INTEGER *serial);
ASN1_INTEGER *osh_X509_get_serialNumber(X509 *x);

int osh_X509_set_issuer_name(X509 *x, X509_NAME *name);
X509_NAME *osh_X509_get_issuer_name(const X509 *a);

int osh_X509_set_subject_name(X509 *x, X509_NAME *name);
X509_NAME *osh_X509_get_subject_name(X509 *a);

//ASN1_TIME * osh_X509_get_notBefore(X509 *x);
int osh_X509_set_notBefore(X509 *x, const ASN1_TIME *tm);

//ASN1_TIME *osh_X509_get_notAfter(X509 *x);
int osh_X509_set_notAfter(X509 *x, const ASN1_TIME *tm);

EVP_PKEY *osh_X509_get_pubkey(X509 *x);
int osh_X509_set_pubkey(X509 *x, EVP_PKEY *pkey);

int osh_X509_get_ext_by_NID(X509 *x, int nid, int lastpos);
X509_EXTENSION *osh_X509_get_ext(X509 *x, int loc);
int osh_X509_add_ext(X509 *x, X509_EXTENSION *ex, int loc);

int osh_X509_sign(X509 *x, EVP_PKEY *pkey, const EVP_MD *md);

ASN1_TIME *osh_X509_gmtime_adj(ASN1_TIME *s, long adj);

int osh_X509_NAME_add_entry_by_txt(X509_NAME *name, const char *field, int type,
                               const unsigned char *bytes, int len, int loc,
                               int set);

// EVP
const EVP_MD *osh_EVP_sha256(void);
EVP_PKEY *osh_EVP_PKEY_new(void);
int osh_EVP_PKEY_assign(EVP_PKEY *pkey, int type, void *key);

// BIGNUM
BIGNUM *osh_BN_new(void);
void osh_BN_free(BIGNUM *a);

int osh_BN_set_word(BIGNUM *a, BN_ULONG w);

// RSA
RSA *osh_RSA_new(void);
void osh_RSA_free(RSA *r);

int osh_i2d_RSAPrivateKey(RSA *rsa, unsigned char **b);

int osh_RSA_generate_key_ex(RSA *rsa, int bits, BIGNUM *e, BN_GENCB *cb);

// ASN1
ASN1_INTEGER *osh_ASN1_INTEGER_new(void);
long osh_ASN1_INTEGER_get(ASN1_INTEGER *a);
int osh_ASN1_INTEGER_set(ASN1_INTEGER *a, long v);

// Extensions
BASIC_CONSTRAINTS *osh_BASIC_CONSTRAINTS_new(void);
X509_EXTENSION *osh_X509V3_EXT_i2d(int ext_nid, int crit, void *ext_struc);


#endif // OPENSSL_SYMBOL_HELPER_H
