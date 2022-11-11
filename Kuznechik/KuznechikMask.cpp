#include <random>
#include <ctime>
#include "KuznechikMask.h"

KuznechikMask::KuznechikMask(const std::vector<unsigned char>& key) :
	Kuznechik(key)
{
}

std::vector<unsigned char> KuznechikMask::encript(const std::vector<unsigned char>& plaintext)
{
	scheduleGenerate();
	masksGenerate();
	Text out = plaintext;
	for (int i = 0; i < sizeof(mask_) / sizeof(mask_[0]); ++i)
		out ^= mask_[i];
	for (int i = 0; i < 9; ++i) {
		out ^= keySchedule_[i] ^ mask_[9 - i];

		for (int j = 0; j < 9 - i; ++j)
			out ^= mask_[j];

		std::mt19937 engine;
		for (int j = 0; j < 255; ++j) {
			Text temp;
			engine.seed(std::time(nullptr));
			temp ^= mask_[engine() % 10];
			temp = sTransform(temp);
		}

		out = sTransform(out);

		Text temp;
		for (int j = 0; j < 9 - i; ++j)
			temp ^= mask_[j];
		out = lTransform(out ^ lTransformReverse(temp));
	}
	out ^= keySchedule_[9] ^ mask_[0];
	return out;
}

std::vector<unsigned char> KuznechikMask::decript(const std::vector<unsigned char>& ciphertext)
{
	scheduleGenerate();
	masksGenerate();
	Text out = ciphertext;
	out ^= keySchedule_[9] ^ lTransform(mask_[0]);
	for (int i = 8; i >= 0; --i) {
		out = lTransformReverse(out);
		for (int j = 0; j < 9 - i; ++j)
			out ^= mask_[j];
		out = sTransformReverse(out);
		Text temp;
		for (int j = 0; j < 9 - i; ++j)
			temp ^= mask_[j];

		out ^= lTransform(temp);

		out ^= keySchedule_[i] ^ lTransform(mask_[9 - i]);
	}
	Text xorAllMasks;
	for (int i = 0; i < sizeof(mask_) / sizeof(mask_[0]); ++i)
		xorAllMasks ^= mask_[i];
	return out ^ lTransform(xorAllMasks);
}

Kuznechik::Text KuznechikMask::rTransform(const Text& data)
{
	unsigned char xorByte = 0;
	for (int i = 0; i < 16; ++i)
		xorByte ^= gfMultiplier(data[i] ^ maskL_[i], lConstans_[i]) ^ gfMultiplier(maskL_[i], lConstans_[i]);

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
		xorByte ^= gfMultiplier(data[i] ^ maskL_[i], lConstans_[15 - i]) ^ gfMultiplier(maskL_[i], lConstans_[15 - i]);
	}
	out[15] = xorByte;
	return out;
}

void KuznechikMask::masksGenerate()
{
	std::mt19937 engine;
	engine.seed(std::time(nullptr));
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < textSize; ++j)
			mask_[i][j] = static_cast<unsigned char>(engine());
		maskL_[i] = static_cast<unsigned char>(engine());
	}
}
