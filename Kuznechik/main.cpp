#include <iostream>
#include <iomanip>
#include "KuznechikMask.h"

template<class OutIt>
void hexPrint(OutIt begin, OutIt end)
{
    std::cout << "0x" << std::setbase(16);
    for (auto i = begin; i != end; ++i)
        std::cout << static_cast<unsigned int>(*i);
}

void doIt(const std::vector<unsigned char>& key, bool decript,
    const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& ciphertext)
{
    const int w = 8;
    std::cout << std::setw(w) << "Key: ";
    hexPrint(key.begin(), key.end());
    std::cout << std::endl;

    std::cout << std::setw(w) << "Text: ";
    hexPrint(plaintext.begin(), plaintext.end());
    std::cout << std::endl;

    KuznechikMask aKuznechik(key);
    aKuznechik.scheduleGenerate();
    std::vector<unsigned char> outputCipher = !decript ? aKuznechik.encript(plaintext) : aKuznechik.decript(plaintext);
    std::cout << std::setw(w) << "Cipher: ";
    hexPrint(outputCipher.begin(), outputCipher.end());
    std::cout << std::endl;

    if (ciphertext == outputCipher)
        std::cout << "OK" << std::endl;
    else
        std::cout << "Fail" << std::endl;
    std::cout << std::endl;
}

int main()
{
    const std::vector<unsigned char> key = {
        0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    };
    const std::vector<unsigned char> plaintext = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
    };
    const std::vector<unsigned char> ciphertext = {
        0x7F, 0x67, 0x9D, 0x90, 0xBE, 0xBC, 0x24, 0x30, 0x5A, 0x46, 0x8D, 0x42, 0xB9, 0xD4, 0xED, 0xCD,
    };

    try {
        doIt(key, false, plaintext, ciphertext);
        doIt(key, true, ciphertext, plaintext);
    }
    catch (const Kuznechik::Exception& exc) {
        std::cout << "Exception: " << exc.what() << std::endl;
    }

    return 0;
}
