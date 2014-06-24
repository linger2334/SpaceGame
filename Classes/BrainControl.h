#ifndef _BRAINCONTROL_H_
#define _BRAINCONTROL_H_

#include "Util.h"

class BrainControl : public Layer
{
public:
	bool init();
	CREATE_FUNC(BrainControl);

	Vector<SpriteFrame *> _animFrames;

	LabelBMFont * labelLives;
	LabelBMFont * labelAttention;
	ProgressTimer *progress1;
	ProgressTimer *progress2;

	Sprite *spriteEyes;
	Repeat *repeatEyes;
	Animate* action;
	Animation *animation;

	void SetLabelLives(int lives,int score);
	bool isHaveSaveFile();  //�жϴ浵�Ƿ����
	int getHighestHistorySorce();
	void setHighestHistorySorce(int score);

	int _lives;
	//�Բ����
	int _Attention;
	int _PoolSignal;
	int _Attack;

	void Brain();
};

#endif

