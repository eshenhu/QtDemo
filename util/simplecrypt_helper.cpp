#include "simplecrypt_helper.h"
#include "util/simplecrypt.h"
#include <QDebug>

static const quint64 KEY_ENCRYPTO = Q_UINT64_C(0xb07d7fc8cf3708c7);

SimpleCrypt_helper::SimpleCrypt_helper()
{

}

bool SimpleCrypt_helper::encrypto(const QByteArray &in, QByteArray &out)
{
    bool result = true;
    SimpleCrypt crypto(KEY_ENCRYPTO); //some random number
    crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
    crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash); //properly protect the integrity of the data

    out = crypto.encryptToByteArray(in);
    if (crypto.lastError() != SimpleCrypt::ErrorNoError) {
      // do something relevant with the cyphertext, such as storing it or sending it over a socket to another machine.
        result = false;
        qCritical() << "Cryper failed with reason of Error";
    }
    return result;
}


bool SimpleCrypt_helper::decrypto(const QByteArray& encrypto, QByteArray& plain)
{
    bool result = true;

    SimpleCrypt crypto(KEY_ENCRYPTO); //same random number: key should match encryption key
    plain = crypto.decryptToByteArray(encrypto);
    if (crypto.lastError() != SimpleCrypt::ErrorNoError) {
      // check why we have an error, use the error code from crypto.lastError() for that
        qCritical() << "Error: Decrypt failed";
        result = false;
    }
    return result;
}
