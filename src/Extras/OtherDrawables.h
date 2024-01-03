//
// OtherDrawables.h
//	Drawable Object Factory
//
// . . .
//
// Created 1/27/19 by Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "PlatformSDL.h"
#include "DrawableSpecifier.h"


class OtherDrawables
{
public:
	OtherDrawables(PlatformSDL& platform)
	{
		pPlatform = &platform;
	}

	int	 iCurrentDemo = -1;

	DrawableProperties& GetNextDrawable();
private:
	static bool updateSpinOnZAxis(GameClock& time);
	static bool updateRayCast(GameClock& time);

	static PlatformSDL* pPlatform;
};

