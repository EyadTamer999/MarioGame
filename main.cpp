#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
using namespace std;

class Cell{
private:
    int x;
    int y;
    char type;
public:
    Cell(){x = 0; y = 0; type = '.';}
    Cell(int x, int y, char type){
        this -> x = x;
        this -> y = y;
        this -> type = type;
    }
    int getY(){return y;}
    int getX(){return x;}
    void setY(int y){this -> y = y;}
    void setX(int x){this -> x = x;}
    char getType(){return type;}
};

class Champion : public Cell{
private:
    int remainingAbilityMoves = 2;
    int hp = 100;
    int score = 0;
public:
    Champion():Cell(0, 0, 'C'){cout<<"Constructor Champion() is called"<<endl;}

    virtual void useAbility(){
        cout<<"Ability x is called"<<endl;
    }

    int getRemainingAbilityMoves(){return remainingAbilityMoves;}
    void setRemainingAbilityMoves(int x){remainingAbilityMoves = x;}
    int getHP(){return hp;}
    void setHP(int hp){this -> hp = hp;}
    int getScore(){return score;}
    void setScore(int score){this -> score = score;}
    virtual void printStats(){
        cout<<"HP: "<< hp<<endl<<"Score: "<<score<<endl<<"Position: ("<<getY()<<","<<getX()<<")"<<endl<<"Ability Uses: "<<remainingAbilityMoves<<endl;
    }
};

class Mario : public Champion{
public:
    Mario():Champion(){cout<<"Constructor Mario() is called"<<endl;}
    void useAbility(){
        cout<<"Mario uses Ability"<<endl;
        setRemainingAbilityMoves(getRemainingAbilityMoves() - 1);
    }

    void printStats(){
        cout<<"HP: "<<getHP()<<endl<<"Score: "<<getScore()<<endl<<"Position: ("<<getY()<<","<<getX()<<")"<<endl<<"Ability Uses: "<<getRemainingAbilityMoves()<<endl<<"Type: Mario"<<endl;
    }
};

class Luigi : public Champion{
public:
    Luigi():Champion(){cout<<"Constructor Luigi() is called"<<endl;}
    void useAbility(){
        cout<<"Luigi uses Ability"<<endl;
        setRemainingAbilityMoves(getRemainingAbilityMoves() - 1);
    }

    void printStats(){
        cout<<"HP: "<<getHP()<<endl<<"Score: "<<getScore()<<endl<<"Position: ("<<getY()<<","<<getX()<<")"<<endl<<"Ability Uses: "<<getRemainingAbilityMoves()<<endl<<"Type: Luigi"<<endl;
    }
};

class Obstacle : public Cell{
private:
    int damage;
public:
    Obstacle(int x, int y):Cell(x, y, 'O'){
        damage = (rand() % 5) + 1;
    }
    virtual void execute(Champion* c){}
    int getDamage(){return damage;}
};

class Bomb : public Obstacle{
public:
    Bomb(int x, int y):Obstacle(x, y){}
    void execute(Champion* c){
        (*c).setHP((*c).getHP() - getDamage());
        if((*c).getHP() < 0)
            (*c).setHP(0);
        cout<<"bomb executed with dmg = "<<getDamage()<<endl;
    }
};

class Thief : public Obstacle{
public:
    Thief(int x, int y):Obstacle(x, y){}
    void execute(Champion* c){
        (*c).setScore((*c).getScore() - getDamage());
        if((*c).getScore() < 0)
            (*c).setScore(0);
        cout<<"thief executed with dmg = "<<getDamage()<<endl;
    }
};

class Gem : public Cell{
private:
    int points;
public:
    Gem(int x, int y):Cell(x, y, 'G'){
        points = (rand() % 5) + 5;
    }
    virtual void execute(Champion* c){}
    int getPoints(){return points;}
};

class Potion : public Gem{
public:
    Potion(int x, int y):Gem(x, y){}
    void execute(Champion* c){
        (*c).setHP((*c).getHP() + getPoints());
        if((*c).getHP() > 100)
            (*c).setHP(100);
        cout<<"Potion executed with points = "<<getPoints()<<endl;
    }
};

class Coin : public Gem{
public:
    Coin(int x, int y):Gem(x, y){}
    void execute(Champion* c){
        (*c).setScore((*c).getScore() + getPoints());
        cout<<"Coin executed with points = "<<getPoints()<<endl;
    }
};

class Map{
private:
    int rows;
    int columns;
    Cell*** map;
public:
    Map(){
        cout<<"Constructor Map(Champion) is called"<<endl;
        rows = 10;
        columns = 10;
        map = new Cell**[rows];
        for(int i = 0; i < rows; i++){
            map[i] = new Cell*[columns];
        }
    }

    void randomizeMap(Champion* champ) {
        cout<<"randomizeMap() is called"<<endl;

        for(int i = 0; i < rows; i++)
            for(int j = 0; j < columns; j++)
                map[i][j] = new Cell();

        map[(*champ).getY()][(*champ).getX()] = champ;

        for (int i = 0; i < 10; i++) {
            int x, y;
            do {
                x = rand() % rows;
                y = rand() % columns;
            } while ((*map[x][y]).getType() != '.');

            map[x][y] = new Bomb(x, y);
        }
        for (int i = 0; i < 10; i++) {
            int x, y;
            do {
                x = rand() % rows;
                y = rand() % columns;
            } while ((*map[x][y]).getType() != '.');

            map[x][y] = new Thief(x, y);
        }
        for (int i = 0; i < 40; i++) {
            int x, y;
            do {
                x = rand() % rows;
                y = rand() % columns;
            } while ((*map[x][y]).getType() != '.');

            if (rand() % 2 == 1)
                map[x][y] = new Potion(x, y);
            else
                map[x][y] = new Coin(x, y);
        }
    }

    void printMap(){
        cout<<"printMap() is called"<<endl;
        for(int i = rows - 1; i >= 0; i--){
            for(int j = 0; j < columns; j++){
                cout<<(*map[i][j]).getType()<<" ";
            }
            cout<<endl;
        }
    }

    Cell*** getMap(){return map;}

    ~Map(){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++)
                delete map[i][j];
            delete map[i];
        }

        delete map;
    }
};

int main() {
    srand(time(NULL));
    Champion* champ;
    char type;
    do {
        cout << "Enter Which champion you want to play as Mario ('M') of Luigi ('L'): ";
        type = _getch();
        system("CLS");
        if (type == 'M')
            champ = new Mario();
        else if (type == 'L')
            champ = new Luigi();
    }while(type != 'M' && type != 'L');
    Map* map = new Map();
    char choice;
    (*map).randomizeMap(champ);
    (*map).printMap();
    do{
        cout << "Would you like to reroll the map? (1 to start or 2 to reroll)" << endl;
        choice = _getch();
        system("CLS");
        if(choice == '2'){
            (*map).randomizeMap(champ);
            (*map).printMap();
        }
    }while(choice != '1');

    system("CLS");
    int newX = 0;
    int newY = 0;
    char input;
    int counterGems;

    while(true){
        (*map).printMap();
        (*champ).printStats();
        Cell*** tmpMap = (*map).getMap();
        cout<<"Enter 8(up), 5(down), 4(left), 6(right) or X to use your ability"<<endl;
        input = _getch();
        if(input == '8'){
            if((*champ).getX() + 1 > 9){
                system("CLS");
                cout << "Not Allowed, Out Of Bounds" << endl;
                continue;
            }
            newX = (*champ).getX() + 1;
        }else if(input == '5'){
            if((*champ).getX() - 1 < 0){
                system("CLS");
                cout << "Not Allowed, Out Of Bounds" << endl;
                continue;
            }
            newX = (*champ).getX() - 1;
        }else if(input == '6'){
            if((*champ).getY() + 1 > 9){
                system("CLS");
                cout << "Not Allowed, Out Of Bounds" << endl;
                continue;
            }
            newY = (*champ).getY() + 1;
        }else if(input == '4'){
            if((*champ).getY() - 1 < 0){
                system("CLS");
                cout << "Not Allowed, Out Of Bounds" << endl;
                continue;
            }
            newY = (*champ).getY() - 1;
        }else if(input == 'X' || input == 'x'){
            if((*champ).getRemainingAbilityMoves() <= 0){
                cout<<"no remaining ability uses"<<endl;
                continue;
            }
            cout<<"Enter the direction required"<<endl;
            char direction = _getch();
            if(direction == '8'){
                if(type == 'M'){
                    if((*champ).getX() + 2 > 9){
                        system("CLS");
                        cout << "Not Allowed, Out Of Bounds" << endl;
                        continue;
                    }
                    newX = (*champ).getX() + 2;
                    (*champ).useAbility();
                }else{
                    int y = (*champ).getY();
                    for(int i = (*champ).getX(); i < 9; i++){
                        if((*tmpMap[i][y]).getType() == 'O'){
                            tmpMap[i][y] = new Cell();
                        }
                    }
                    (*champ).useAbility();
                }
            }else if(direction == '5'){
                if(type == 'M'){
                    if((*champ).getX() - 2 < 0){
                        system("CLS");
                        cout << "Not Allowed, Out Of Bounds" << endl;
                        continue;
                    }
                    newX = (*champ).getX() - 2;
                    (*champ).useAbility();
                }else{
                    int y = (*champ).getY();
                    for(int i = (*champ).getX(); i >= 0; i--){
                        if((*tmpMap[i][y]).getType() == 'O'){
                            tmpMap[i][y] = new Cell();
                        }
                    }
                    (*champ).useAbility();
                }
            }else if(direction == '6'){
                if(type == 'M'){
                    if((*champ).getY() + 2 > 9){
                        system("CLS");
                        cout << "Not Allowed, Out Of Bounds" << endl;
                        continue;
                    }
                    newY = (*champ).getY() + 2;
                    (*champ).useAbility();
                }else{
                    int x = (*champ).getX();
                    for(int i = (*champ).getY(); i < 9; i++){
                        if((*tmpMap[x][i]).getType() == 'O'){
                            tmpMap[x][i] = new Cell();
                        }
                    }
                    (*champ).useAbility();
                }
            }else if(direction == '4'){
                if(type == 'M'){
                    if((*champ).getY() - 2 < 0){
                        system("CLS");
                        cout << "Not Allowed, Out Of Bounds" << endl;
                        continue;
                    }
                    newY = (*champ).getY() - 2;
                    (*champ).useAbility();
                }else{
                    int x = (*champ).getX();
                    for(int i = (*champ).getY(); i >= 0; i--){
                        if((*tmpMap[x][i]).getType() == 'O'){
                            tmpMap[x][i] = new Cell();
                        }
                    }
                    (*champ).useAbility();
                }
            }
        }

        system("CLS");

        if((*tmpMap[newX][newY]).getType() == 'O'){
            Obstacle* tmp = static_cast<Obstacle*>(tmpMap[newX][newY]);
            (*tmp).execute(champ);
        }else if((*tmpMap[newX][newY]).getType() == 'G'){
            Gem* tmp = static_cast<Gem*>(tmpMap[newX][newY]);
            (*tmp).execute(champ);
            counterGems++;
        }
        cout<<"Gems collected: "<<counterGems<<endl;
        tmpMap[(*champ).getX()][(*champ).getY()] = new Cell();
        (*champ).setX(newX);
        (*champ).setY(newY);
        tmpMap[newX][newY] = champ;

        if((*champ).getHP() == 0){
            cout<<"Game Over"<<endl;
            break;
        }else if(counterGems == 40){
            cout<<"You Win"<<endl;
            break;
        }
    }

    delete map;
}
