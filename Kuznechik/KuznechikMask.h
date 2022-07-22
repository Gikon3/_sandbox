#pragma once
#include "Kuznechik.h"

class KuznechikMask : public Kuznechik
{
public:
	KuznechikMask() = delete;
	KuznechikMask(const std::vector<unsigned char>& key);
	~KuznechikMask() override = default;
	std::vector<unsigned char> encript(const std::vector<unsigned char>& plaintext) override;
	std::vector<unsigned char> decript(const std::vector<unsigned char>& ciphertext) override;

protected:
	Text mask_[10];
	Text maskL_;

private:
	Text rTransform(const Text& data) override;
	Text rTransformReverse(const Text& data) override;
};
