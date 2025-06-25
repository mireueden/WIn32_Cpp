#include "iQueueKey.h"

iQueueKey::iQueueKey()
{
    qk = NULL;
    qkNum = 0;
}

iQueueKey* iQueueKey::share()
{
	static iQueueKey* instance = new iQueueKey();
	return instance; 
}

iQueueKey::~iQueueKey()
{
	delete qk;
}

void iQueueKey::set(int maxNUm, METHOD_KEY m)
{
	if (qk)
		delete qk;
	qk = new QueueKey[1000];
	qkNum = 0;

	method = m;
}

void iQueueKey::add(iKeyStat s, iPoint p)
{
    qk[qkNum].s = s;
    qk[qkNum].p = p;
    qkNum++;
}

void iQueueKey::update(float dt)
{
    if (qkNum == 0)
        return;

    QueueKey* q = &qk[0];
    method(q->s, q->p);

    qkNum--;
    memcpy(qk, &qk[1], sizeof(QueueKey) * qkNum);
}

// ===================================
// Keyboard
// ===================================

iKeyboard::iKeyboard()
{
    keydown = keydown_none;
    keystat = keydown_none;
}

iKeyboard* iKeyboard::share()
{
    static iKeyboard* instance = new iKeyboard();
    return instance;
}

iKeyboard::~iKeyboard()
{


}

void iKeyboard::add(bool down, int parm)
{
    if (down)
    {
        add(true, keystat, parm);
        add(true, keydown, parm);
    }
    else// if (down == false)
    {
        add(false, keystat, parm);
    }
}

void iKeyboard::add(bool pressed, int& keydown, int key)
{
    int keys[][2] = {
        {87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        //{87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        //{87, keydown_w}, {65, keydown_a}, {83, keydown_s}, {68, keydown_d},
        {32, keydown_space},
    };
    int nKey = sizeof(keys) / sizeof(int) / 2;

    if (pressed)
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown |= keys[i][1];
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < nKey; i++)
        {
            if (key == keys[i][0])
            {
                keydown &= ~keys[i][1];
                break;
            }
        }
    }
}

void iKeyboard::update(float dt)
{
    keydown = keydown_none;
}

bool iKeyboard::keyStat(int key)
{
    return keystat & key;
}

bool iKeyboard::keyDown(int key)
{
    return keydown & key;
}