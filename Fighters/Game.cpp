//
//  Game.cpp
//  Fighters
//
//  Created by Jimmy Fan on 2017/5/15.
//  Copyright © 2017年 Jimmy Fan. All rights reserved.
//

#include <cstdio>
#include <cstring>
#include "Game.h"
using namespace std;

Game::Game() {
    mWindow.create(sf::VideoMode(windowWidth,windowHeight), "Fighters");
    mWindow.setFramerateLimit(60);
    gameTime = sf::Time::Zero;
    bossTime = sf::Time::Zero;
    font.loadFromFile("/Users/fjm/ccpp2017/Fighters/Resources/Arial Black.ttf");
    gameMusic.openFromFile("/Users/fjm/ccpp2017/Fighters/Resources/game_music.wav");
    gameOverMusic.openFromFile("/Users/fjm/ccpp2017/Fighters/Resources/game_over.wav");
    enemyDownBuffer.loadFromFile("/Users/fjm/ccpp2017/Fighters/Resources/enemy0_down.wav");
    enemyDownSound.setBuffer(enemyDownBuffer);
    playerDownBuffer.loadFromFile("/Users/fjm/ccpp2017/Fighters/Resources/player_down.wav");
    playerDownSound.setBuffer(playerDownBuffer);
}

void Game::run() {
    sf::Clock clock;
    sf::Time time = sf::Time::Zero;
    sf::Time Time = sf::seconds(1.f/60.f);
    sf::Time dTime;
    gameMusic.play();
//    boss.bossComing = 1;
    while (mWindow.isOpen()) {
        processEvents();
        dTime = clock.getElapsedTime();
        gameTime += dTime;
        if (boss.bossTime) bossTime += dTime;
        time += clock.restart();
        while (time > Time) {
            time -= Time;
            processEvents();
            if (!player.GameOver) update(Time);
        }
        render();
    }
    gameOverMusic.stop();
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            handlePlayerInput(event.key.code, 1);
        }
        if (event.type == sf::Event::KeyReleased) {
            handlePlayerInput(event.key.code, 0);
        }
    }
}

void Game::update(sf::Time time) {
    player.move(time);
    if (!boss.bossComing) {
        enemy.creatEnemy(gameTime);
    }
    enemy.shoot(gameTime);
    enemy.move(time);
    checkCrash();
    if (boss.bossTime) {
        boss.move(time);
        boss.shoot(bossTime, player);
    }
    else {
        if (boss.bossComing) {
            bool flag = 1;
            for (int i = 0; i < enemy.numOfEnemys; i++) {
                if (enemy.enemys[i].aliveCondition || enemy.enemys[i].shot.isAlive) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                boss.bossTime = 1;
            }
        }
    }
}

void Game::render() {
    mWindow.clear();
    if (player.GameOver) {
        gameMusic.stop();
        gameOverMusic.play();
        sf::Text over;
        over.setFont(font);
        over.setString("Game Over");
        over.setStyle(sf::Text::Bold);
        over.setPosition((windowWidth - over.getLocalBounds().width)/2, (windowHeight - over.getLocalBounds().height)/2);
        over.setFillColor(sf::Color::White);
        mWindow.draw(over);
        over.setString("Enter to RESTART");
        over.setPosition((windowWidth - over.getLocalBounds().width)/2, (windowHeight - over.getLocalBounds().height)/2 + 100);
        mWindow.draw(over);
        mWindow.display();
        return;
    }
    if (boss.WIN) {
        sf::Text win;
        win.setFont(font);
        win.setString("YOU WIN!!");
        win.setStyle(sf::Text::Bold);
        win.setPosition((windowWidth - win.getLocalBounds().width)/2, (windowHeight - win.getLocalBounds().height)/2);
        win.setFillColor(sf::Color::White);
        mWindow.draw(win);
        win.setString("Enter to RESTART");
        win.setPosition((windowWidth - win.getLocalBounds().width)/2, (windowHeight - win.getLocalBounds().height)/2 + 100);
        mWindow.draw(win);
        mWindow.display();
        return;
    }
    mWindow.draw(score.score);
    if (player.aliveCondition) mWindow.draw(player.Plane);
    for (int i = 0; i < numOfShots; i++) {
        if (player.shots[i].isAlive) {
            mWindow.draw(player.shots[i].shot);
        }
    }
    for (int i = 0; i < enemy.numOfEnemys; i++) {
        if (enemy.enemys[i].aliveCondition) {
            mWindow.draw(enemy.enemys[i].enemyPlane);
        }
    }
    for (int i = 0; i < enemy.numOfEnemys; i++) {
        if (enemy.enemys[i].shot.isAlive) {
            mWindow.draw(enemy.enemys[i].shot.shot);
        }
    }
    for (int i = 1; i <= player.life; i++) {
        heart.heart.setPosition(windowWidth - i * (20 + heart.getRect().x), 20);
        mWindow.draw(heart.heart);
    }
    if (boss.bossTime) {
        mWindow.draw(boss.Plane);
        for (int i = 0; i < numOfBossShots; i++) {
            if (boss.shots[i].isAlive) {
                mWindow.draw(boss.shots[i].shot);
            }
        }
    }
    mWindow.display();
}

void Game::reStart() {
    gameTime = sf::Time::Zero;
    bossTime = sf::Time::Zero;
    score.updateScore(0);
    player.reStart();
    enemy.reStart();
    boss.reStart();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Up || key == sf::Keyboard::Down || key == sf::Keyboard::Left || key == sf::Keyboard::Right) {
        player.setMove(key,isPressed);
    }
    if (key == sf::Keyboard::Space) {
        player.shoot(isPressed);
    }
    if (key == sf::Keyboard::Return) {
        if (player.GameOver || boss.WIN) {
            reStart();
        }
    }
}

bool Game::isCollsion(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    if (x1 >= x2 && x1 >= x2 + w2) return 0;
    if (x1 <= x2 && x1 + w1 <= x2) return 0;
    if (y1 >= y2 && y1 >= y2 + h2) return 0;
    if (y1 <= y2 && y1 + h1 <= y2) return 0;
    return 1;
}

void Game::checkCrash() {
    for (int i = 0; i < enemy.numOfEnemys; i++) {
        if (enemy.enemys[i].aliveCondition != 1) continue;
        for (int j = 0; j < numOfShots; j++) {
            if (!player.shots[j].isAlive) continue;
            sf::Vector2f posEnemy = enemy.enemys[i].enemyPlane.getPosition();
            sf::Vector2f posShot = player.shots[j].shot.getPosition();
            if (isCollsion(posEnemy.x, posEnemy.y, enemy.enemys[i].rect.x, enemy.enemys[i].rect.y, posShot.x, posShot.y, player.shots[j].rect.x, player.shots[j].rect.y)) {
                enemy.enemys[i].down(1);
                player.shots[j].isAlive = 0;
                enemyDownSound.play();
                score.updateScore(10);
                if (score.getScore() > 30) {
                    boss.bossComing = 1;
                }
                break;
            }
        }
    }
    if (boss.bossTime) {
        if (boss.aliveCondition == 1) {
            for (int i = 0; i < numOfShots; i++) {
                if (!player.shots[i].isAlive) continue;
                sf::Vector2f posBoss = boss.Plane.getPosition();
                sf::Vector2f posShot = player.shots[i].shot.getPosition();
                if (isCollsion(posBoss.x, posBoss.y, boss.rect.x, boss.rect.y, posShot.x, posShot.y, player.shots[i].rect.x, player.shots[i].rect.y)) {
                    boss.HP --;
                    if (boss.HP == 0) {
                        boss.down(1);
                    }
                    player.shots[i].isAlive = 0;
                    break;
                }
            }
        }
    }
    if (player.aliveCondition != 1) return;
    for (int i = 0; i < enemy.numOfEnemys; i++) {
        if (enemy.enemys[i].aliveCondition != 1) continue;
        sf::Vector2f posPlayer = player.Plane.getPosition();
        sf::Vector2f posEnemy = enemy.enemys[i].enemyPlane.getPosition();
        if (isCollsion(posPlayer.x, posPlayer.y, player.rect.x, player.rect.y, posEnemy.x, posEnemy.y, enemy.enemys[i].rect.x, enemy.enemys[i].rect.y)) {
                enemy.enemys[i].down(1);
                playerDownSound.play();
                player.down(1);
                break;
        }
    }
    if (player.aliveCondition != 1) return;
    for (int i = 0; i < enemy.numOfEnemys; i++) {
        if (enemy.enemys[i].shot.isAlive) {
            sf::Vector2f posPlayer = player.Plane.getPosition();
            sf::Vector2f posShot = enemy.enemys[i].shot.shot.getPosition();
            if (isCollsion(posPlayer.x, posPlayer.y, player.rect.x, player.rect.y, posShot.x, posShot.y, enemy.enemys[i].shot.rect.x, enemy.enemys[i].shot.rect.y)) {
                    enemy.enemys[i].shot.isAlive = 0;
                    playerDownSound.play();
                    player.down(1);
                    break;
            }
        }
    }
    if (player.aliveCondition != 1) return;
    for (int i = 0; i < numOfBossShots; i++) {
        if (boss.shots[i].isAlive) {
            sf::Vector2f posPlayer = player.Plane.getPosition();
            sf::Vector2f posShot = boss.shots[i].shot.getPosition();
            if (isCollsion(posPlayer.x, posPlayer.y, player.rect.x, player.rect.y, posShot.x, posShot.y, boss.shots[i].rect.x, boss.shots[i].rect.y)) {
                boss.shots[i].isAlive = 0;
                playerDownSound.play();
                player.down(1);
                break;
            }
        }
    }
    if (player.aliveCondition != 1) return;
    sf::Vector2f posBoss = boss.Plane.getPosition();
    sf::Vector2f posPlayer = player.Plane.getPosition();
    if (isCollsion(posPlayer.x, posPlayer.y, player.rect.x, player.rect.y, posBoss.x, posBoss.y, boss.rect.x, boss.rect.y)) {
        playerDownSound.play();
        player.down(1);
    }
}
