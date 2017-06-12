//
//  Menu.h
//  Fighters
//
//  Created by Jimmy Fan on 2017/6/12.
//  Copyright © 2017年 Jimmy Fan. All rights reserved.
//

#ifndef Menu_h
#define Menu_h

#include <SFML/Graphics.hpp>

class Menu {
private:
    sf::Font font;
    sf::Text Damage;
    sf::Text Speed;
    sf::Text Life;
public:
    Menu();
    void setData(int damage, int speed, int life);
    sf::Text getDamageText();
    sf::Text getSpeedText();
    sf::Text getLifeText();
};

#endif /* Menu_h */
