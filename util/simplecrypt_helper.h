#ifndef SIMPLECRYPT_HELPER_H
#define SIMPLECRYPT_HELPER_H

#include <QByteArray>

class SimpleCrypt_helper
{
public:
    SimpleCrypt_helper();

public:
    static bool encrypto(const QByteArray &in, QByteArray &out);
    static bool decrypto(const QByteArray& encrypto, QByteArray& plain);
};

#endif // SIMPLECRYPT_HELPER_H
