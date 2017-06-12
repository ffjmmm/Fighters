//
//  Player.h
//  Fighters
//
//  Created by Jimmy Fan on 2017/5/15.
//  Copyright © 2017年 Jimmy Fan. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include "Sprite.h"
#include "Shot.h"
#include <SFML/Audio.hpp>
#define numOfShots 20

class Player: public Sprite {
private:
    bool moveUp,moveDown,moveLeft,moveRight;
    sf::SoundBuffer shotBuffer;
    sf::Sound shotSound;
public:
    int life;
    sf::Sprite Plane;
    int aliveCondition;
    Shot shots[numOfShots];
    Player();
    bool GameOver;
    void setMove(sf::Keyboard::Key key, bool isPressed);
    void move(sf::Time time);
    void shoot(bool isPressed);
    void down(int k);
    void reStart();
};


#endif /* Player_h */
