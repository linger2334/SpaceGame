#include "BrainControl.h"

bool BrainControl::init()
{
	Layer::init();

	labelLives = LabelBMFont::create("", "fonts/Arial.fnt");
	labelLives->setScale(0.5);
	labelLives->setPosition(winSize.width*0.6, winSize.height*0.92);
	this->addChild(labelLives);

	//attention  ��ʼ��ע�����ı�ǩ
	labelAttention = LabelBMFont::create("0", "fonts/Arial.fnt");
	labelAttention->setScale(0.5);
	labelAttention->setPosition(winSize.width*0.1, winSize.height*0.92);
	this->addChild(labelAttention);

	//��ʼ��ע����������
	progress1 = ProgressTimer::create(Sprite::create("attention2.png"));
	progress1->setScale(0.3);
	progress1->setPosition(winSize.width*0.1, winSize.height*0.92);

	//���ý���������ʽ
	progress1->setType(kCCProgressTimerTypeRadial);

	//���ý���ֵ��Χ[0,100]
	progress1->setPercentage(100);

	//�����ȼ�ʱ
	progress1->setReverseProgress(true);
	this->addChild(progress1);

	//��ʼ���Բ��źŽ�����
	progress2 = ProgressTimer::create(Sprite::create("attention.png"));
	progress2->setScale(0.5);
	progress2->setPosition(winSize.width*0.95, winSize.height*0.95);
	//���ý���������ʽ
	progress2->setType(kCCProgressTimerTypeBar);
	//���ü�ʱ����ʼ��
	progress2->setMidpoint(ccp(0, 0));
	//���ü�ʱ������
	progress2->setBarChangeRate(ccp(1, 0));
	this->addChild(progress2);

	//���գ�۾���Ч��,��ʼ��գ��ʱ��Ҫ��ʾ��ͼƬԪ��
	//auto *pTexture = TextureCache::sharedTextureCache()->addImage("eyesm.png");
	auto* pNewBatchNode = SpriteBatchNode::create("eyesm.png", 2);
	addChild(pNewBatchNode, 1);
	pNewBatchNode->getTexture()->setAliasTexParameters();
	auto *pTexture = pNewBatchNode->getTexture();
	auto *frame0 = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 147, 147));
	auto *frame1 = SpriteFrame::createWithTexture(pTexture, CCRectMake(147, 0, 147, 147));
	_animFrames.pushBack(frame0);
	_animFrames.pushBack(frame1);

	_lives = 20;

	//�Բ����
	_Attention = 0;
	_PoolSignal = 0;
	_Attack = 30;
	return true;
}

void BrainControl::Brain()
{
	int iAttention = 0;
	int iPoolSignal = 0;
	int iBlink = 0;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	//void doStuff();
	int getAttention();
	int getPoorSignal();
	int getBlink();

	//doStuff();
	iPoolSignal = getPoorSignal();
	iBlink = getBlink();
	iAttention = getAttention();
	_Attack = iAttention;
#else
	iAttention = 100;
#endif
	if (_PoolSignal > 100 - iPoolSignal / 2)
		_PoolSignal--;
	else if (_PoolSignal < 100 - iPoolSignal / 2)
		_PoolSignal++;
	progress2->setPercentage(_PoolSignal);

	//�ж��ź�
	if (_PoolSignal < 10)
		return;

	if (iAttention < 5)
		return;

	if (iBlink)
	{
		iBlink = 0;

		//SimpleAudioEngine::getInstance()->playEffect("laser_enemy.wav");
		_lives--;
		iAttention = 30;
		_Attack = 30;


		//գ�۾����Ŷ���
		animation = Animation::createWithSpriteFrames(_animFrames, 0.2f);
		animation->setDelayPerUnit(0.2f);
		animation->setRestoreOriginalFrame(true);
		action = Animate::create(animation);

		spriteEyes = Sprite::create();
		spriteEyes->setPosition(ccp(winSize.width*0.25, winSize.height*0.92));
		spriteEyes->stopAllActions();
		repeatEyes = Repeat::create(action, 1);
		spriteEyes->setScaleX(0.4f);
		spriteEyes->setScaleY(0.32f);
		spriteEyes->runAction(repeatEyes);
		addChild(spriteEyes, 1);
	}

	if (_Attention > iAttention)
		_Attention--;
	else if (_Attention < iAttention)
		_Attention++;

	char szAttention[100];
	sprintf(szAttention, "%3d", _Attention);
	labelAttention->setString(szAttention);
	progress1->setPercentage(_Attention);
}

void BrainControl::SetLabelLives(int lives, int score)
{
	char szLives[100];
	sprintf(szLives, "highest score:%3d | lives:%2d | score:%3d", getHighestHistorySorce(), lives, score);
	labelLives->setString(szLives);
}

bool BrainControl::isHaveSaveFile()//�жϴ浵�Ƿ����
{
	if (!UserDefault::getInstance()->getBoolForKey("isHaveSaveFileXml"))//ͨ�����õ�bool�ͱ�־λ�жϣ����������
	{
		UserDefault::getInstance()->setBoolForKey("isHaveSaveFileXml", true);//д��bool�ж�λ
		UserDefault::getInstance()->setIntegerForKey("HighestScore", 0);//д���ʼ����0
		UserDefault::getInstance()->flush();//������һ��Ҫ����flush�����ܴӻ���д��io
		return false;
	}
	else
	{
		return true;
	}
}

int BrainControl::getHighestHistorySorce()
{
	if (isHaveSaveFile()) //������ڴ浵
	{
		return UserDefault::getInstance()->getIntegerForKey("HighestScore", 0);//��ȡ��ʷ��߷�
	}
	return 0;
}

void BrainControl::setHighestHistorySorce(int score)
{
	int lScore = getHighestHistorySorce();
	if (score > lScore)
	{
		UserDefault::getInstance()->setIntegerForKey("HighestScore", score);//�޸Ĵ浵
	}
}
