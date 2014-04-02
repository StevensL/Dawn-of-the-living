#pragma once
#include "gui.h"

class Bite
{
public:
	Bite::Bite(const ModelMD2& biteModel, const Texture2D& explosionTexInit, World* world, const Direction& newDirection);
	Bite(void);
	~Bite(void);

	bool _isJawsClosed;
	GUI* JawsClosed;
};
