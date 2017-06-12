//
//  Menu.cpp
//  Fighters
//
//  Created by Jimmy Fan on 2017/6/12.
//  Copyright © 2017年 Jimmy Fan. All rights reserved.
//

#include "Menu.h"
#include <cstdio>
using namespace std;

Menu::Menu() {
    font.loadFromFile("/Users/fjm/Git/Fighters/Resources/Arial Black.ttf");
    Damage.setFont(font);
    Damage.setStyle(sf::Text::Bold);
    Speed.setFont(font);
    Speed.setStyle(sf::Text::Bold);
    Life.setFont(font);
    Life.setStyle(sf::Text::Bold);
}

void Menu::setData(int damage, int speed, int life) {
    Damage.setString("Damamge:    " + to_string(damage));
    Speed.setString( "Speed:          " + to_string(speed/50));
    Life.setString(  "Life:              " + to_string(life));
    Damage.setPosition(50, 100);
    Speed.setPosition(50, 200);
    Life.setPosition(50, 300);
}

sf::Text Menu::getDamageText() {
    return Damage;
}

sf::Text Menu::getSpeedText() {
    return Speed;
}

sf::Text Menu::getLifeText() {
    return Life;
}
