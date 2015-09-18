//
//  ProgressBar.h
//  MaruOtoshi
//
//  Created by JoHazumu on 2015/09/18.
//
//

#ifndef __MaruOtoshi__ProgressBar__
#define __MaruOtoshi__ProgressBar__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class ProgressBar : public Layer
{
public:
    ProgressBar();
    virtual ~ProgressBar();
    bool init() override;

    void update(float dt) override;
    void setWidth(float rate);
    
    Sprite* bar;
    CREATE_FUNC(ProgressBar);
};


#endif /* defined(__MaruOtoshi__ProgressBar__) */
