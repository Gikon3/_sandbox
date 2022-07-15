#include <random>
#include <ctime>
#include "KuznechikMask.h"

KuznechikMask::KuznechikMask(const std::vector<unsigned char>& key) :
	Kuznechik(key)
{
	std::mt19937 engine;
	engine.seed(std::time(nullptr));
	for (int i = 0; i < textSize; ++i) {
		mask_[0][i] = static_cast<unsigned char>(engine());
		mask_[1][i] = static_cast<unsigned char>(engine());
		mask_[2][i] = static_cast<unsigned char>(engine());
	}
}

std::vector<unsigned char> KuznechikMask::encript(const std::vector<unsigned char>& plaintext)
{
	scheduleGenerate();
	Text out = plaintext ^ mask_[0] ^ mask_[1];
	for (int i = 0; i < 9; ++i) {
		out ^= keySchedule_[i] ^ mask_[1];
		out = sTransform(out ^ mask_[0]);
		out = lTransform(out ^ lTransformReverse(mask_[0] ^ mask_[1]));
	}
	out ^= keySchedule_[9] ^ mask_[0] ^ mask_[1];
	return out;
}

std::vector<unsigned char> KuznechikMask::decript(const std::vector<unsigned char>& ciphertext)
{
	scheduleGenerate();
	Text out = ciphertext ^ lTransform(mask_[0]);
	out ^= keySchedule_[9] ^ lTransform(mask_[1]);
	for (int i = 8; i >= 0; --i) {
		out = lTransformReverse(out);
		out = sTransformReverse(out ^ mask_[0] ^ mask_[1]);
		out ^= lTransform(mask_[0]);
		out ^= keySchedule_[i] ^ lTransform(mask_[1]);
	}
	return out ^ lTransform(mask_[0]) ^ lTransform(mask_[1]);
}

Kuznechik::Text KuznechikMask::rTransform(const Text& data)
{
	unsigned char xorByte = 0;
	for (int i = 0; i < 16; ++i)
		xorByte ^= gfMultiplier(data[i] ^ mask_[2][i], lConstans_[i]) ^ gfMultiplier(mask_[2][i], lConstans_[i]);

	Text out;
	out[0] = xorByte;
	for (int i = 0; i < 15; ++i)
		out[i + 1] = data[i];
	return out;
}

Kuznechik::Text KuznechikMask::rTransformReverse(const Text& data)
{
	Text out;
	unsigned char xorByte = gfMultiplier(data[15], lConstans_[0]);
	for (int i = 0; i < 15; ++i) {
		out[i] = data[i + 1];
		xorByte ^= gfMultiplier(data[i] ^ mask_[2][i], lConstans_[15 - i]) ^ gfMultiplier(mask_[2][i], lConstans_[15 - i]);
	}
	out[15] = xorByte;
	return out;
}
