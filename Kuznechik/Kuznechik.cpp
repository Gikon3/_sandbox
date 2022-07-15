#include "Kuznechik.h"

#define MASTER_KEY_SIZE 32
#define TEXT_SIZE       16
#define SCHEDULE_SIZE   10

Kuznechik::Text::Text() :
	text_(TEXT_SIZE)
{
}

Kuznechik::Text::Text(const std::vector<unsigned char>& text)
{
	if (text.size() != TEXT_SIZE)
		throw "Size";
	text_ = text;
}

Kuznechik::Text::Text(const unsigned char* text) :
	text_(TEXT_SIZE)
{
	for (int i = 0; i < text_.size(); ++i)
		text_[i] = text[i];
}

Kuznechik::Text& Kuznechik::Text::operator^=(const Text& rhs)
{
	for (int i = 0; i < text_.size(); ++i)
		text_[i] ^= rhs.text_[i];
	return *this;
}

unsigned char Kuznechik::Text::operator[](int index) const
{
	return text_[index];
}

unsigned char& Kuznechik::Text::operator[](int index)
{
	return text_[index];
}

size_t Kuznechik::Text::size() const
{
	return text_.size();
}

Kuznechik::Kuznechik(const std::vector<unsigned char>& key) :
	scheduleValid_(false), keySchedule_(2)
{
	if (key.size() != MASTER_KEY_SIZE)
		throw "Size";

	for (int i = 0; i < TEXT_SIZE; ++i) {
		keySchedule_[0][i] = key[i];
		keySchedule_[1][i] = key[TEXT_SIZE + i];
	}
}

void Kuznechik::scheduleGenerate()
{
	if (!scheduleValid_) {
		Text keyA = keySchedule_[0];
		Text keyB = keySchedule_[1];
		for (int i = 0; i < 32; ++i) {
			Text temp = keyA;
			temp ^= iterConstans_[i];
			temp = sTransform(temp);
			temp = lTransform(temp);
			temp ^= keyB;
			keyB = keyA;
			keyA = temp;
			if ((i + 1) % 8 == 0) {
				keySchedule_.push_back(keyA);
				keySchedule_.push_back(keyB);
			}
		}
		scheduleValid_ = true;
	}
}

std::vector<unsigned char> Kuznechik::encript(const std::vector<unsigned char>& plaintext)
{
	scheduleGenerate();
	Text out = plaintext.data();
	for (int i = 0; i < 9; ++i) {
		out ^= keySchedule_[i];
		out = sTransform(out);
		out = lTransform(out);
	}
	out ^= keySchedule_[9];
	std::vector<unsigned char> outVector(16);
	for (int i = 0; i < out.size(); ++i)
		outVector[i] = out[i];
	return outVector;
}

std::vector<unsigned char> Kuznechik::decript(const std::vector<unsigned char>& ciphertext)
{
	scheduleGenerate();
	Text out = ciphertext.data();
	out ^= keySchedule_[9];
	for (int i = 8; i >= 0; --i) {
		out = lTransformReverse(out);
		out = sTransformReverse(out);
		out ^= keySchedule_[i];
	}
	std::vector<unsigned char> outVector(16);
	for (int i = 0; i < out.size(); ++i)
		outVector[i] = out[i];
	return outVector;
}

Kuznechik::Text Kuznechik::sTransform(const Text& data)
{
	Text out;
	for (int i = 0; i < out.size(); ++i)
		out[i] = pi_[data[i]];
	return out;
}

Kuznechik::Text Kuznechik::sTransformReverse(const Text& data)
{
	Text out;
	for (int i = 0; i < out.size(); ++i)
		out[i] = piReverse_[data[i]];
	return out;
}

Kuznechik::Text Kuznechik::rTransform(const Text& data)
{
	unsigned char xorByte = 0;
	for (int i = 0; i < 16; ++i)
		xorByte ^= gfMultiplier(data[i], lConstans_[i]);

	Text out;
	out[0] = xorByte;
	for (int i = 0; i < 15; ++i)
		out[i + 1] = data[i];
	return out;
}

Kuznechik::Text Kuznechik::rTransformReverse(const Text& data)
{
	unsigned char xorByte = gfMultiplier(data[15], lConstans_[0]);
	Text out;
	for (int i = 0; i < 15; ++i) {
		out[i] = data[i + 1];
		xorByte ^= gfMultiplier(data[i], lConstans_[15 - i]);
	}
	out[15] = xorByte;
	return out;
}

Kuznechik::Text Kuznechik::lTransform(const Text& data)
{
	Text out = data;
	for (int i = 0; i < out.size(); ++i)
		out = rTransform(out);
	return out;
}

Kuznechik::Text Kuznechik::lTransformReverse(const Text& data)
{
	Text out = data;
	for (int i = 0; i < out.size(); ++i)
		out = rTransformReverse(out);
	return out;
}

unsigned char Kuznechik::gfMultiplier(unsigned char a, unsigned char b)
{
	unsigned char out = 0;
	for (int i = 0; i < 8; ++i) {
		if ((b & 1) == 1)
			out ^= a;
		const char hiBit = a & 0x80;
		a <<= 1;
		if (hiBit < 0)
			a ^= 0xC3;
		b >>= 1;
	}
	return out;
}

unsigned char Kuznechik::gfPow(unsigned char value, int n)
{
	if (n == 0)
		return 1;

	if (n % 2 == 0)
		return gfPow(gfMultiplier(value, value), n / 2);

	unsigned char square = gfMultiplier(value, value);
	return gfMultiplier(gfPow(square, n / 2), value);
}

unsigned char Kuznechik::gfInverse(unsigned char value)
{
	return gfPow(value, 254);
}
