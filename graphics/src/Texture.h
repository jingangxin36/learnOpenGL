#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int _rendererID;
	std::string _filePath;
	unsigned char* _localBuffer;
	int _width, _height, _BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};