////////////////////////////////////////////////////////////////////////////////
/// @brief OpenSSL class to interface libssl
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2014 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Oreste Costa-Panaia
/// @author Copyright 2014, ArangoDB GmbH, Cologne, Germany
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#include "SslInterface.h"

#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#include "Basics/RandomGenerator.h"
#include "Basics/StringUtils.h"

using namespace std;
using namespace triagens::basics;

// -----------------------------------------------------------------------------
// helper
// -----------------------------------------------------------------------------

namespace {
  static Random::UniformCharacter SaltGenerator("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*(){}[]:;<>,.?/|");
}

namespace triagens {
  namespace rest {
    namespace SslInterface {

// -----------------------------------------------------------------------------
      // public methods
// -----------------------------------------------------------------------------

      void sslMD5 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[MD5_DIGEST_LENGTH];
          outputLen = MD5_DIGEST_LENGTH;
        }

        MD5((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }



      void sslMD5 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslMD5(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslMD5 (char const* input1, size_t length1, char const* input2, size_t length2, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[MD5_DIGEST_LENGTH];
          outputLen = MD5_DIGEST_LENGTH;
        }

        MD5_CTX ctx;

        MD5_Init(&ctx);
        MD5_Update(&ctx, input1, length1);
        MD5_Update(&ctx, input2, length2);
        MD5_Final((unsigned char*) outputStr, &ctx);
      }



      void sslSHA1 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[SHA_DIGEST_LENGTH];
          outputLen = SHA_DIGEST_LENGTH;
        }

        SHA1((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }



      void sslSHA1 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslSHA1(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslSHA224 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[SHA224_DIGEST_LENGTH];
          outputLen = SHA224_DIGEST_LENGTH;
        }

        SHA224((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }



      void sslSHA224 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslSHA224(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslSHA256 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[SHA256_DIGEST_LENGTH];
          outputLen = SHA256_DIGEST_LENGTH;
        }

        SHA256((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }


      void sslSHA256 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslSHA256(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslSHA384 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[SHA384_DIGEST_LENGTH];
          outputLen = SHA384_DIGEST_LENGTH;
        }

        SHA384((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }



      void sslSHA384 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslSHA384(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslSHA512 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        if (outputStr == 0) {
          outputStr = new char[SHA512_DIGEST_LENGTH];
          outputLen = SHA512_DIGEST_LENGTH;
        }

        SHA512((const unsigned char*) inputStr, length, (unsigned char*) outputStr);
      }



      void sslSHA512 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslSHA512(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslHEX (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        static char const hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

        if (outputStr == 0) {
          outputStr = new char[length * 2 + 1];
          outputLen = length * 2;
        }

        char const * e = inputStr + length;
        char * p = outputStr;

        for (char const * q = inputStr;  q < e;  ++q) {
          *p++ = hexval[(*q >> 4) & 0xF];
          *p++ = hexval[*q & 0x0F];
        }

        *p = '\0';
      }



      void sslHEX (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslHEX(inputStr, strlen(inputStr), outputStr, outputLen);
      }



      void sslBASE64 (char const* inputStr, size_t length, char*& outputStr, size_t& outputLen) {
        string b = StringUtils::encodeBase64(string(inputStr, length));

        if (outputStr == 0) {
          outputStr = new char[b.size() + 1];
          outputLen = length * 2;
        }

        memcpy(outputStr, b.c_str(), outputLen);
      }



      void sslBASE64 (char const* inputStr, char*& outputStr, size_t& outputLen) {
        sslBASE64(inputStr, strlen(inputStr), outputStr, outputLen);
      }


      string sslPBKDF2 (char const* salt, size_t saltLength, char const* pass, size_t passLength, int iter, int keyLength) {
        unsigned char* dk = (unsigned char*) TRI_SystemAllocate(EVP_MAX_MD_SIZE + 1, false);

        PKCS5_PBKDF2_HMAC_SHA1(pass, (int) passLength, (const unsigned char*) salt, (int) saltLength, iter, keyLength, dk);

        // return value as hex
        string result = StringUtils::encodeHex(string((char*)dk, keyLength));
        TRI_SystemFree(dk);

        return result;
      }


      string sslHMAC (char const* key, size_t keyLength, char const* message, size_t messageLen, Algorithm algorithm) {
        EVP_MD* evp_md = nullptr;

        if (algorithm == Algorithm::ALGORITHM_SHA1) {
          evp_md = const_cast<EVP_MD*>(EVP_sha1());
        }
        else if (algorithm == Algorithm::ALGORITHM_SHA224) {
          evp_md = const_cast<EVP_MD*>(EVP_sha224());
        }
        else if (algorithm == Algorithm::ALGORITHM_MD5) {
          evp_md = const_cast<EVP_MD*>(EVP_md5());
        }
        else if (algorithm == Algorithm::ALGORITHM_SHA384) {
          evp_md = const_cast<EVP_MD*>(EVP_sha384());
        }
        else if (algorithm == Algorithm::ALGORITHM_SHA512) {
          evp_md = const_cast<EVP_MD*>(EVP_sha512());
        }
        else {
          // default
          evp_md = const_cast<EVP_MD*>(EVP_sha256());
        }

        unsigned char* md = (unsigned char*) TRI_SystemAllocate(EVP_MAX_MD_SIZE + 1, false);
        unsigned int md_len;

        HMAC(evp_md, key, (int) keyLength, (const unsigned char*) message, messageLen, md, &md_len);

        // return value as hex
        string result = StringUtils::encodeHex(string((char*)md, md_len));
        TRI_SystemFree(md);

        return result;
      }


      bool verifyHMAC (char const* challenge, size_t challengeLength, char const* secret, size_t secretLen, char const* response, size_t responseLen, Algorithm algorithm) {
        // challenge = key
        // secret, secretLen = message
        // result must == BASE64(response, responseLen)

        string s = sslHMAC(challenge, challengeLength, secret, secretLen, algorithm);

        if (s.length() == responseLen && s.compare( string(response, responseLen) )  == 0) {
          return true;
        }

        return false;
      }


      int sslRand (uint64_t* value) {
        if (! RAND_pseudo_bytes((unsigned char *) value, sizeof(uint64_t))) {
          return 1;
        }

        return 0;
      }

      int sslRand (int64_t* value) {
        if (! RAND_pseudo_bytes((unsigned char *) value, sizeof(int64_t))) {
          return 1;
        }

        return 0;
      }

      int sslRand (int32_t* value) {
        if (! RAND_pseudo_bytes((unsigned char *) value, sizeof(int32_t))) {
          return 1;
        }

        return 0;
      }

      void salt64 (uint64_t& result) {
        string salt = SaltGenerator.random(8);
        char* saltChar = const_cast<char*>(salt.c_str());
        uint64_t* saltInt = reinterpret_cast<uint64_t*>(saltChar);
        result = *saltInt;
      }



      void saltChar (char*& result, size_t length) {
        if (length == 0) {
          result = 0;
          return;
        }

        string salt = SaltGenerator.random(length);
        result = StringUtils::duplicate(salt);
      }
    }
  }
}
// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
