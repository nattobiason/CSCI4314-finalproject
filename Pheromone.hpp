// Pheromone class
#include "LTexture.hpp"

class Pheromone{
    public:
        Pheromone(int, int, double, bool);
        double update_concentration();
        LTexture get_color();
        int mPosX, mPosY;
        void render_p();
        bool home_trail;

    private:
        double concentration;
         // 1 if pheromones are from home to food, and 0 if trail is from food to home
        

};

Pheromone::Pheromone(int x, int y, double w, bool b){
    mPosX = x;
    mPosY = y;
    concentration = w;
    home_trail = b;
};

double Pheromone::update_concentration(){
    concentration *= (0.98);
    return concentration;
}


void Pheromone::render_p(){
    if(home_trail){
        gHomeTrailTexture.render(mPosX, mPosY);
    }
    else{
        gFoodTrailTexture.render(mPosX, mPosY);
    }
}