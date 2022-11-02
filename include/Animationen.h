#pragma once

#include "Arduino.h"
#include <NeoPixelAnimator.h>

extern NeoPixelAnimator blendToBlack;

const byte ANIM_VALUES[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
const String ANIM_NAMES[] = {"Keine", "Wischen", "Doppel-Wischen", "Feuer", "Implosion", "Explosion", "Impl-Expl", "Schreiben", "Falling Letters"};



//void BlendToBlackUpdate(const AnimationParam& param);
//void FireWorksUpdate(const AnimationParam& param);
//
//void startBlendToBlack(int pixel);

void playAnimation();
void noAnimation();

void fireworks();
void christmasTree();

class Animationen
{
public:
	Animationen();
	~Animationen();
};

// Fading durch Hinzufügen eines jeden Pixels in ein Array, ein Fade-Funktion durchläuft jedes Pixel bis es aus ist und entfernt es dann aus dem array
