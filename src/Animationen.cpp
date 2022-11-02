#include "Animationen.h"
#include "Arduino.h"
#include "LedStrip.h"
#include "wortuhr_func.h"
#include "config.h"


void noAnimation();
void wipe();
void doubleWipe();
void fire();
void in();
void out();
void inOut();
void typing();
void risingLetters();

bool isClockletterPosition(int position);
void blowupRocket(int position);



void playAnimation() {
	switch (GLOBAL::Animation) {
	case ANIMATION_OFF:
		//Log("Animation: off");
		noAnimation();
		break;
	case ANIMATION_WIPE:
		//Log("Animation: wipe");
		wipe();
		break;
	case ANIMATION_DOUBLEWIPE:
		//Log("Animation: doublewipe");
		doubleWipe();
		break;
	case ANIMATION_FIRE:
		//Log("Animation: fire");
		fire();
		break;
	case ANIMATION_IN:
		//Log("Animation: in");
		in();
		break;
	case ANIMATION_OUT:
		//Log("Animation: out");
		out();
		break;
	case ANIMATION_INOUT:
		//Log("Animation: in-out");
		inOut();
		break;
	case ANIMATION_TYPING:
		//Log("Animation: typing");
		typing();
		break;
	case ANIMATION_RISING:
		//Log("Animation: rising letters");
		risingLetters();
		break;
	}
	clockLetters.clear();
}

void noAnimation() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	std::list<int>::iterator it = clockLetters.begin();
	for (int i = 0; i <= 109; i++) {
		bool found = false;
		for (const int position : clockLetters) {
			if (i == position) {
				found = true;
				break;
			}
		}
		if (found) {
			strip.SetPixelColor(i, color);
		} else {
			strip.SetPixelColor(i, black);
		}
	}
	strip.Show();
}

void wipe() {
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);

	RgbwColor before = animColor;
	before.Lighten(80);
	//before.Darken(180);
	for (int col = 0; col <= 12; col++) {
		for (int row = 0; row <= 9; row++) {

			if (col - 2 >= 0 && col - 2 <= 10) {
				bool found = false;
				for (const int position : clockLetters) {
					if (tiles.Map(col - 2, row) == position) {
						found = true;
						break;
					}
				}
				if (found) {
					SetPixelColor(tiles.Map(col - 2, row), color, true);
				}
				else {
					SetPixelColor(tiles.Map(col - 2, row), black, true);
				}

			}
			
			
			if (col - 1 >= 0 && col - 1 <= 10) {
				SetPixelColor(tiles.Map(col - 1, row), before, true);
			}
			
			
			if (col >= 0 && col <= 10) {
				SetPixelColor(tiles.Map(col, row), animColor, true);
			}
			
		}
		//delay(50);
		Show();
	}
}

void doubleWipe() {
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);

	RgbwColor before = animColor;
	before.Lighten(80);
	//before.Darken(180);
	RgbwColor after = animColor;
	after.Darken(40);

	for (int col = 0; col <= 12; col++) {
		for (int row = 0; row <= 9; row++) {
			if (col - 2 >= 0 && col - 2 <= 10) {
				SetPixelColor(tiles.Map(col - 2, row), black, true);
			}

			if (col - 1 >= 0 && col - 1 <= 10) {
				SetPixelColor(tiles.Map(col - 1, row), before, true);
			}
			if (col >= 0 && col <= 10) {
				SetPixelColor(tiles.Map(col, row), animColor, true);
			}
		}
		Show();
	}
	for (int col = 12; col >= -2; col--) {
		for (int row = 0; row <= 9; row++) {
			if (col + 2 >= 0 && col + 2 <= 10) {
				bool found = false;
				for (const int position : clockLetters) {
					if (tiles.Map(col + 2, row) == position) {
						found = true;
						break;
					}
				}
				if (found) {
					SetPixelColor(tiles.Map(col + 2, row), color, true);
				}
				else {
					SetPixelColor(tiles.Map(col + 2, row), black, true);
				}
			}
			
			if (col + 1 >= 0 && col + 1 <= 10) {
				SetPixelColor(tiles.Map(col + 1, row), before, true);
			}
			
			if (col >=0 && col <= 10) {
				SetPixelColor(tiles.Map(col, row), animColor, true);
			}
		}
		Show();
	}
}

void fire() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);

	for (int h = 9; h > 0; h--) {
		for (int c = 0; c < 5; c++) {
			//clearLeds();
			for (int i = 0; i < 11; i++) {
				//int max_px = random(random(h - 3, h), 9);
				int max_px = random(h , h + 4);
				if (max_px > 9) max_px = 9;
				strip.SetPixelColor(tiles.Map(i, max_px), red);
				RgbwColor lowerred = strip.GetPixelColor(tiles.Map(i, max_px));
				lowerred.Darken(random(80, 140));
				strip.SetPixelColor(tiles.Map(i, max_px - 1), lowerred);
				strip.SetPixelColor(tiles.Map(i, max_px + 1), red);
				strip.SetPixelColor(tiles.Map(i, max_px + 2), RgbColor(255, 60, 0));
				int red_height = random(2, 3);
				if (red_height == 3) strip.SetPixelColor(tiles.Map(i, max_px + 3), RgbColor(255, 60, 0));
				for (int t = max_px + 1 + red_height; t <= 9; t++) {
					strip.SetPixelColor(tiles.Map(i, t), RgbColor(255, 120, 0));
				}
				for (int t = max_px - 1; t >= 0; t--) {

					RgbwColor darker = strip.GetPixelColor(tiles.Map(i, t));
					darker.Darken(50);
					strip.SetPixelColor(tiles.Map(i, t), darker);
					//strip.SetPixelColor(tiles.Map(i, t), black);
				}
			}
			strip.Show();
			delay(150);
		}
	}
	for (int h = 0; h < 10; h++) {
		for (int c = 0; c < 5; c++) {
			//clearLeds();
			for (int i = 0; i < 11; i++) {
				//int max_px = random(random(h - 3, h), 9);
				int max_px = random(h, h + 4);
				if (max_px > 9) max_px = 9;
				strip.SetPixelColor(tiles.Map(i, max_px), red);
				RgbwColor lowerred = strip.GetPixelColor(tiles.Map(i, max_px));
				lowerred.Darken(random(80, 140));
				strip.SetPixelColor(tiles.Map(i, max_px - 1), lowerred);
				strip.SetPixelColor(tiles.Map(i, max_px + 1), red);
				strip.SetPixelColor(tiles.Map(i, max_px + 2), RgbColor(255, 60, 0));
				int red_height = random(2, 3);
				if (red_height == 3) strip.SetPixelColor(tiles.Map(i, max_px + 3), RgbColor(255, 60, 0));
				for (int t = max_px + 1 + red_height; t <= 9; t++) {
					strip.SetPixelColor(tiles.Map(i, t), RgbColor(255, 120, 0));
				}
				for (int t = max_px - 1; t >= 0; t--) {
					bool found = false;
					for (const int position : clockLetters) {
						if (tiles.Map(i, t) == position) {
							found = true;
							break;
						}
					}
					if (found) {
						strip.SetPixelColor(i, color);
					}
					else {
						RgbwColor darker = strip.GetPixelColor(tiles.Map(i, t));
						darker.Darken(50);
						strip.SetPixelColor(tiles.Map(i, t), darker);
						//strip.SetPixelColor(tiles.Map(i, t), black);
					}
				}
			}
			strip.Show();
			delay(150);
		}
	}
	//TODO: noch nicht funktionstüchtig
}

void in() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	for (int i = 0; i <=7; i++) {
		for (int col = 0; col <= 10; col++) {
			if (col == i || col == 10 - i) {
				for (int row = 0; row <= 9; row++) {
					if (row >= i && row <= 9 - i) {
						SetPixelColor(tiles.Map(col, row), animColor, true);
					}
					else {
						if (!(row > i && row <= 9 - i && col >= i && col <= 10 - i)) {
							bool found = false;
								for (const int position : clockLetters) {
									if (tiles.Map(col, row) == position) {
										found = true;
											break;
									}
								}
							if (found) {
								SetPixelColor(tiles.Map(col, row), color, true);
							}
							else {
								SetPixelColor(tiles.Map(col, row), black, true);
							}
						}
					}
				}
			}
			else {
				for (int row = 0; row <= 9; row++) {
					if (!(row > i && row <= 9 - i && col >= i && col <= 10 - i)) {
						bool found = false;
						for (const int position : clockLetters) {
							if (tiles.Map(col, row) == position) {
								found = true;
								break;
							}
						}
						if (found) {
							SetPixelColor(tiles.Map(col, row), color, true);
						}
						else {
							SetPixelColor(tiles.Map(col, row), black, true);
						}
					}
				}
			}
			
		}
		for (int row = 0; row <= 9; row++) {
			if (row == i || row == 9 - i) {
				for (int col = 0; col <= 10; col++) {
					if (col >= i && col <= 10 - i) {
						SetPixelColor(tiles.Map(col, row), animColor);
					} else {
						if (!(row > i && row <= 9 - i && col >= i && col <= 10 - i)) {
							bool found = false;
							for (const int position : clockLetters) {
								if (tiles.Map(col, row) == position) {
									found = true;
									break;
								}
							}
							if (found) {
								SetPixelColor(tiles.Map(col, row), color, true);
							}
							else {
								SetPixelColor(tiles.Map(col, row), black, true);
							}
						}
					}
				}
			}
		}
		Show();
	}

}

void out() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	for (int i = 5; i >= -3; i--) {
		for (int col = 0; col <= 10; col++) {
			if (col == i || col == 10 - i) {
				for (int row = 0; row <= 9; row++) {
					if (row >= i && row <= 9 - i) {
						SetPixelColor(tiles.Map(col, row), animColor, true);
					}
					else {
						if (row > i && row < 9 - i && col > i && col < 10 - i) {
							bool found = false;
							for (const int position : clockLetters) {
								if (tiles.Map(col, row) == position) {
									found = true;
									break;
								}
							}
							if (found) {

								SetPixelColor(tiles.Map(col, row), color, true);

							}
							else {
								SetPixelColor(tiles.Map(col, row), black, true);
							}
						}
					}
				}
			}
			else {
				for (int row = 0; row <= 9; row++) {
					if (row > i && row < 9 - i && col > i && col < 10 - i) {
						bool found = false;
						for (const int position : clockLetters) {
							if (tiles.Map(col, row) == position) {
								found = true;
								break;
							}
						}
						if (found) {

							SetPixelColor(tiles.Map(col, row), color, true);

						}
						else {
							SetPixelColor(tiles.Map(col, row), black, true);
						}
					}
				}
			}
		}

		for (int row = 0; row <= 9; row++) {
			if (row == i || row == 9 - i) {
				for (int col = 0; col <= 10; col++) {
					if (col >= i && col <= 10 - i) {
						SetPixelColor(tiles.Map(col, row), animColor);
					}
					else {
						if (row > i && row < 9 - i && col > i && col < 10 - i) {
							bool found = false;
							for (const int position : clockLetters) {
								if (tiles.Map(col, row) == position) {
									found = true;
									break;
								}
							}
							if (found) {

								SetPixelColor(tiles.Map(col, row), color, true);

							}
							else {
								SetPixelColor(tiles.Map(col, row), black, true);
							}
						}
					}
				}
			}
		}
		Show();
	}
}

void inOut() {
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	for (int i = 0; i <= 6; i++) {
		for (int col = 0; col <= 10; col++) {
			if (col == i || col == 10 - i) {
				for (int row = 0; row <= 9; row++) {
					if (row >= i && row <= 9 - i) {
						SetPixelColor(tiles.Map(col, row), animColor, true);
					}else {
						SetPixelColor(tiles.Map(col, row), black, true);
					}
				}
			} else {
				for (int row = 0; row <= 9; row++) {
					if (row < i || row > 9 - i) {
						SetPixelColor(tiles.Map(col, row), black, true);
					}
				}
			}
		}
		
		for (int row = 0; row <= 9; row++) {
			if (row == i || row == 9 - i) {
				for (int col = 0; col <= 10; col++) {
					if (col >= i && col <= 10 - i) {
						SetPixelColor(tiles.Map(col, row), animColor);
					} else {
						SetPixelColor(tiles.Map(col, row), black, true);
					}
				}
			} else {
				for (int col = 0; col <= 10; col++) {
					if (col < i || col > 10 - i) {
						SetPixelColor(tiles.Map(col, row), black, true);
					}
				}
			}
		}
		Show();
	}
	out();
}

void typing() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	for (int i = 0; i <= 109; i++) {
		SetPixelColor(i, black, true);
	}
	Show();
	for (const int position : clockLetters) {
		SetPixelColor(position, animColor, false);
		Show();
		delay(75);
		SetPixelColor(position, color);
		Show();
		delay(150);
	}
}

void risingLetters() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	for (int i = 0; i <= 109; i++) {
		SetPixelColor(i, black, true);
	}
	Show();
	for (const int position : clockLetters) {
		int y = GetTop(position) - 1;
		int x = GetLeft(position) - 1;
		Log("x: " + String(x) + ", y: " + String(y));

		for (int iy = 10; iy>=y;iy--) {

			if (iy < 10) SetPixelColor(tiles.Map(x, iy +1), black,true);
			if (iy == y) {
				SetPixelColor(tiles.Map(x,iy), color, true);
			} else {
				SetPixelColor(tiles.Map(x,iy), animColor, true);
			}
			
			Show();
			delay(50);
		}
	}
}

void spiral() {
	RgbwColor color = RgbwColor(GLOBAL::ColorR, GLOBAL::ColorG, GLOBAL::ColorB, GLOBAL::ColorW);
	RgbwColor animColor = RgbwColor(GLOBAL::AnimationColorR,GLOBAL::AnimationColorG,GLOBAL::AnimationColorB,GLOBAL::AnimationColorW);
	int x = 0;
	int y = 0;
	for(int p = 0;p<5;p++){
		for(y=p;y<tiles.getHeight();y++) {
			SetPixelColor(tiles.Map(x,y), animColor, false);
			Show();
			delay(75);
			if (isClockletterPosition(tiles.Map(x,y))) {
				SetPixelColor(tiles.Map(x,y), color);
			} else {
				SetPixelColor(tiles.Map(x,y), black);
			}
			Show();
			delay(150);
		}
		for(x=p;x<tiles.getWidth();y++) {
			SetPixelColor(tiles.Map(x,y), animColor, false);
			Show();
			delay(75);
			if (isClockletterPosition(tiles.Map(x,y))) {
				SetPixelColor(tiles.Map(x,y), color);
			} else {
				SetPixelColor(tiles.Map(x,y), black);
			}
			Show();
			delay(150);
		}
		for(y=tiles.getHeight()-p;y>=p;y--) {
			SetPixelColor(tiles.Map(x,y), animColor, false);
			Show();
			delay(75);
			if (isClockletterPosition(tiles.Map(x,y))) {
				SetPixelColor(tiles.Map(x,y), color);
			} else {
				SetPixelColor(tiles.Map(x,y), black);
			}
			Show();
			delay(150);
		}
		for(x=tiles.getWidth()-p;x>=p+1;x--) {
			SetPixelColor(tiles.Map(x,y), animColor, false);
			Show();
			delay(75);
			if (isClockletterPosition(tiles.Map(x,y))) {
				SetPixelColor(tiles.Map(x,y), color);
			} else {
				SetPixelColor(tiles.Map(x,y), black);
			}
			Show();
			delay(150);
		}
	}


}

void fireworks() {
	Log("start fireworks");
	const int anzahlPositions = 30;
	int positions[anzahlPositions] = {35, 36, 37, 38, 39, 40, 47, 48, 49, 50, 51, 52, 57, 58, 59, 60, 61, 62, 69, 70, 71, 72, 73, 74, 79, 80, 81, 82, 83, 84};
	
	for (int i = 0; i <= 113; i++) {
		SetPixelColor(i, black, true);
	}
	Show();
	for (int i = 0;i<=60;i++) {
		blowupRocket(positions[random(0,anzahlPositions)]);
		delay(random(50,800));
	}
	wordClock(true);
}

void christmasTree() {
	clearLeds();
	// tree
	SetPixelColor(tiles.Map(5,0), white);
	SetPixelColor(tiles.Map(5,1), green);

	//SetPixelColor(tiles.Map(4,2), green);
	//SetPixelColor(tiles.Map(6,2), green);

	//SetPixelColor(tiles.Map(4,3), green);
	//SetPixelColor(tiles.Map(6,3), green);

	for(int i=4;i<=6;i++) {
		SetPixelColor(tiles.Map(i,2), green);
		SetPixelColor(tiles.Map(i,3), green);
	}

	//SetPixelColor(tiles.Map(3,4), green);
	//SetPixelColor(tiles.Map(7,4), green);

	//SetPixelColor(tiles.Map(3,5), green);
	//SetPixelColor(tiles.Map(7,5), green);
	for(int i=3;i<=7;i++) {
		SetPixelColor(tiles.Map(i,4), green);
		SetPixelColor(tiles.Map(i,5), green);
	}

	//SetPixelColor(tiles.Map(2,6), green);
	//SetPixelColor(tiles.Map(8,6), green);

	//SetPixelColor(tiles.Map(2,7), green);
	//SetPixelColor(tiles.Map(8,7), green);

	//SetPixelColor(tiles.Map(3,7), green);
	//SetPixelColor(tiles.Map(4,7), green);
	//SetPixelColor(tiles.Map(5,7), green);
	//SetPixelColor(tiles.Map(6,7), green);
	//SetPixelColor(tiles.Map(7,7), green);
	for(int i=2;i<=8;i++) {
		SetPixelColor(tiles.Map(i,6), green);
		SetPixelColor(tiles.Map(i,7), green);
	}

	SetPixelColor(tiles.Map(5,8), brown);
	SetPixelColor(tiles.Map(5,9), brown);

	// balls
	SetPixelColor(tiles.Map(5,2), red);
	SetPixelColor(tiles.Map(4,4), blue);
	SetPixelColor(tiles.Map(5,5), orange);
	SetPixelColor(tiles.Map(3,6), yellow);
	SetPixelColor(tiles.Map(6,6), red);

	Show();
	
}

void blowupRocket(int position) {
	int x = GetLeft(position) - 1;
	int y = GetTop(position) - 1;
	const int colorCnt = 5;
	int maxSize = 1;
	RgbwColor colors[colorCnt] = {red,blue,green,orange,yellow};
	RgbwColor color = colors[random(0,colorCnt)];
	RgbwColor smileColor = RgbwColor(50,128,255);

	// sicherstellen, dass die Größe passt und nicht über den Rand der Uhr hinausgeht
	if (x >= 4 && x <= 7 && y >= 4 && y <= 6){
		maxSize = 5;
	} else if( x >= 3 && x <= 8 && y >= 3 && y <= 7) {
		maxSize = 4;
	} else if( x >= 2 && x <= 9 && y >= 2 && y <= 8) {
		maxSize = 3;
	}else if ( x >= 1 && x <= 10 && y >= 1 && y <= 9) {
		maxSize = 2;
	} else {
		maxSize = 1;
	}

	int size = random(2,maxSize);

	for (int i = tiles.getHeight();i>=y;i--) {
		if (i > y) SetPixelColor(tiles.Map(x,i+1),black);
		SetPixelColor(tiles.Map(x,i),white);
		Show();
		delay(50);
	}

	if (size >= 1) {
		SetPixelColor(position, white);
		SetPixelColor(tiles.Map(x+1,y),color);
		SetPixelColor(tiles.Map(x-1,y),color);
		SetPixelColor(tiles.Map(x,y+1),color);
		SetPixelColor(tiles.Map(x,y-1),color);
		Show();
		delay(20);
		SetPixelColor(position, black, true);
		Show();
		if (size == 1) {
			delay(380);
		} else {
			delay(80);
		}
		SetPixelColor(tiles.Map(x+1,y),black, true);
		SetPixelColor(tiles.Map(x-1,y),black, true);
		SetPixelColor(tiles.Map(x,y+1),black, true);
		SetPixelColor(tiles.Map(x,y-1),black, true);
		Show();
	}
	if (size >= 2) {
		SetPixelColor(tiles.Map(x + 1, y + 1), color);
		SetPixelColor(tiles.Map(x + 1, y - 1), color);
		SetPixelColor(tiles.Map(x - 1, y - 1), color);
		SetPixelColor(tiles.Map(x - 1, y + 1), color);

		SetPixelColor(tiles.Map(x + 2, y), color);
		SetPixelColor(tiles.Map(x - 2, y), color);
		SetPixelColor(tiles.Map(x, y + 2), color);
		SetPixelColor(tiles.Map(x, y - 2), color);
		Show();
		if (size == 2) {
			delay(400);
		} else {
			delay(100);
		}
		SetPixelColor(tiles.Map(x + 1, y + 1), black, true);
		SetPixelColor(tiles.Map(x + 1, y - 1), black, true);
		SetPixelColor(tiles.Map(x - 1, y - 1), black, true);
		SetPixelColor(tiles.Map(x - 1, y + 1), black, true);

		SetPixelColor(tiles.Map(x + 2, y), black, true);
		SetPixelColor(tiles.Map(x - 2, y), black, true);
		SetPixelColor(tiles.Map(x, y + 2), black, true);
		SetPixelColor(tiles.Map(x, y - 2), black, true);
		Show();
		

	}
	if (size >= 3) {
		SetPixelColor(tiles.Map(x, y + 2), color);
		SetPixelColor(tiles.Map(x, y - 2), color);
		SetPixelColor(tiles.Map(x + 1, y + 2), color);
		SetPixelColor(tiles.Map(x + 1, y - 2), color);
		SetPixelColor(tiles.Map(x + 2, y), color);
		SetPixelColor(tiles.Map(x + 2, y - 1), color);
		SetPixelColor(tiles.Map(x + 2, y + 1), color);
		SetPixelColor(tiles.Map(x - 1, y + 2), color);
		SetPixelColor(tiles.Map(x - 1, y - 2), color);
		SetPixelColor(tiles.Map(x - 2, y), color);
		SetPixelColor(tiles.Map(x - 2, y - 1), color);
		SetPixelColor(tiles.Map(x - 2, y + 1), color);
		Show();
		if (size == 3) {
			delay(400);
		} else {
			delay(100);
		}
		SetPixelColor(tiles.Map(x, y + 2), black, true);
		SetPixelColor(tiles.Map(x, y - 2), black, true);
		SetPixelColor(tiles.Map(x + 1, y + 2), black, true);
		SetPixelColor(tiles.Map(x + 1, y - 2), black, true);
		SetPixelColor(tiles.Map(x + 2, y), black, true);
		SetPixelColor(tiles.Map(x + 2, y - 1), black, true);
		SetPixelColor(tiles.Map(x + 2, y + 1), black, true);
		SetPixelColor(tiles.Map(x - 1, y + 2), black, true);
		SetPixelColor(tiles.Map(x - 1, y - 2), black, true);
		SetPixelColor(tiles.Map(x - 2, y), black, true);
		SetPixelColor(tiles.Map(x - 2, y - 1), black, true);
		SetPixelColor(tiles.Map(x - 2, y + 1), black, true);
		Show();
	}
	if (size >= 4) {
		// zufällig als Herz beenden, wenn Farbe rot
		if (random(0,2) && color == red) {
			SetPixelColor(tiles.Map(x, y - 1), color);
			SetPixelColor(tiles.Map(x + 1, y - 2), color);
			SetPixelColor(tiles.Map(x + 2, y - 2), color);
			SetPixelColor(tiles.Map(x + 3, y - 1), color);
			SetPixelColor(tiles.Map(x + 3, y), color);
			SetPixelColor(tiles.Map(x + 2, y + 1), color);
			SetPixelColor(tiles.Map(x + 1, y + 2), color);
			SetPixelColor(tiles.Map(x, y + 3), color);
			SetPixelColor(tiles.Map(x - 1, y + 2), color);
			SetPixelColor(tiles.Map(x - 2, y + 1), color);
			SetPixelColor(tiles.Map(x - 3, y), color);
			SetPixelColor(tiles.Map(x - 3, y - 1), color);
			SetPixelColor(tiles.Map(x - 2, y - 2), color);
			SetPixelColor(tiles.Map(x - 1, y - 2), color);
			Show();
			if (size == 4) {
				delay(400);
			} else {
				delay(100);
			}
			SetPixelColor(tiles.Map(x, y - 1), black, true);
			SetPixelColor(tiles.Map(x + 1, y - 2), black, true);
			SetPixelColor(tiles.Map(x + 2, y - 2), black, true);
			SetPixelColor(tiles.Map(x + 3, y - 1), black, true);
			SetPixelColor(tiles.Map(x + 3, y), black, true);
			SetPixelColor(tiles.Map(x + 2, y + 1), black, true);
			SetPixelColor(tiles.Map(x + 1, y + 2), black, true);
			SetPixelColor(tiles.Map(x, y + 3), black, true);
			SetPixelColor(tiles.Map(x - 1, y + 2), black, true);
			SetPixelColor(tiles.Map(x - 2, y + 1), black, true);
			SetPixelColor(tiles.Map(x - 3, y), black, true);
			SetPixelColor(tiles.Map(x - 3, y - 1), black, true);
			SetPixelColor(tiles.Map(x - 2, y - 2), black, true);
			SetPixelColor(tiles.Map(x - 1, y - 2), black, true);
			Show();
		} else {
			SetPixelColor(tiles.Map(x, y + 3), color);
			SetPixelColor(tiles.Map(x, y - 3), color);
			SetPixelColor(tiles.Map(x + 1, y + 3), color);
			SetPixelColor(tiles.Map(x + 1, y - 3), color);
			SetPixelColor(tiles.Map(x + 2, y + 2), color);
			SetPixelColor(tiles.Map(x + 2, y - 2), color);
			SetPixelColor(tiles.Map(x + 3, y), color);
			SetPixelColor(tiles.Map(x + 3, y - 1), color);
			SetPixelColor(tiles.Map(x + 3, y + 1), color);
			SetPixelColor(tiles.Map(x - 1, y + 3), color);
			SetPixelColor(tiles.Map(x - 1, y - 3), color);
			SetPixelColor(tiles.Map(x - 2, y + 2), color);
			SetPixelColor(tiles.Map(x - 2, y - 2), color);
			SetPixelColor(tiles.Map(x - 3, y), color);
			SetPixelColor(tiles.Map(x - 3, y - 1), color);
			SetPixelColor(tiles.Map(x - 3, y + 1), color);
			// zufällig mit Smiley füllen
			if (random(0,2)) {
				SetPixelColor(tiles.Map(x + 1, y + 1), smileColor);
				SetPixelColor(tiles.Map(x - 1, y - 1), smileColor);
				SetPixelColor(tiles.Map(x - 1, y + 1), smileColor);
				SetPixelColor(tiles.Map(x + 1, y - 1), smileColor);
				SetPixelColor(tiles.Map(x, y + 2), smileColor);
			}

			Show();
			if (size == 4) {
				delay(400);
			} else {
				delay(100);
			}
			SetPixelColor(tiles.Map(x, y + 3), black, true);
			SetPixelColor(tiles.Map(x, y - 3), black, true);
			SetPixelColor(tiles.Map(x + 1, y + 3), black, true);
			SetPixelColor(tiles.Map(x + 1, y - 3), black, true);
			SetPixelColor(tiles.Map(x + 2, y + 2), black, true);
			SetPixelColor(tiles.Map(x + 2, y - 2), black, true);
			SetPixelColor(tiles.Map(x + 3, y), black, true);
			SetPixelColor(tiles.Map(x + 3, y - 1), black, true);
			SetPixelColor(tiles.Map(x + 3, y + 1), black, true);
			SetPixelColor(tiles.Map(x - 1, y + 3), black, true);
			SetPixelColor(tiles.Map(x - 1, y - 3), black, true);
			SetPixelColor(tiles.Map(x - 2, y + 2), black, true);
			SetPixelColor(tiles.Map(x - 2, y - 2), black, true);
			SetPixelColor(tiles.Map(x - 3, y), black, true);
			SetPixelColor(tiles.Map(x - 3, y - 1), black, true);
			SetPixelColor(tiles.Map(x - 3, y + 1), black, true);

			SetPixelColor(tiles.Map(x + 1, y + 1), black, true);
			SetPixelColor(tiles.Map(x - 1, y - 1), black, true);
			SetPixelColor(tiles.Map(x - 1, y + 1), black, true);
			SetPixelColor(tiles.Map(x + 1, y - 1), black, true);
			SetPixelColor(tiles.Map(x, y + 2), black, true);
			Show();
		}
	}
}

bool isClockletterPosition(int position) {
	for (const int pos : clockLetters) {
		if (pos == position) return true;
	}
	return false;
}

Animationen::Animationen()
{
}


Animationen::~Animationen()
{
}
