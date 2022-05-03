
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Pheromone.hpp"
# define pi           3.14159265358979323846
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



// random float between 0 and 1
float r(){
    return ((float) rand()/RAND_MAX);
}

double minimum_p = 0.5;

//The dot that will move around on the screen
class Dot{
    public:
		// random walk variables
		static const float w; // weight given to directional bias
		static const float theta; // the width of the random walk turning angle distribution (the lower it is, the more straight the trajectory will be)
		static const float BRW_theta;
		static const float CRW_theta;


		//The dimensions of the dot
		static const int DOT_WIDTH = 10;
		static const int DOT_HEIGHT = 10;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		bool b; // 0 if rendering food pheromones, 1 if rendering home pheromones
		bool otw_home; // if 1 responds to home pheromones, if 0 responds to food pheromones

		//Initializes the variables and allocates particles
		Dot();

		void random_walks(double, int,int);

		void unit_vector();
		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		void move(double, double);

		//Shows the dot on the screen
		void render();

        void render_trail();

		double W(double sigma);

		int get_x();
		int get_y();
		int get_vx();
		int get_vy();

		// void biased_random_walks(double, double);


    private:
		// sensitivity of ants to pheromones -- controll degree of randomness for ant following the trail -- the higher the number the more likley the ant will follow
		double beta;
		// pheromone equation constant
		double delta;

		double half_angle;

		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;

		double bias;


};

const float Dot::w = 0.75; // weight given to directional bias
const float Dot::theta = pi/3; // the width of the random walk turning angle distribution (the lower it is, the more straight the trajectory will be)
const float Dot::BRW_theta = pi/3;
const float Dot::CRW_theta = pi/2;


Dot::Dot(){
    //Initialize the offsets
    mPosX = 150;
    mPosY = 400;

    //Initialize the velocity
    mVelX = -1;
    mVelY = 0;

	beta = 3.5;
	delta = 0.2;

	half_angle = pi/3;

	b = 1;
	otw_home = 0;

	bias = 0;

}


void Dot::random_walks(double w, int x_sign, int y_sign){
	mVelX += DOT_VEL * ( w * cos(BRW_theta * 2.0 * r() + bias) + ((1-w) * cos(CRW_theta * 2.0 * r()))  ) * x_sign;
	mVelY += DOT_VEL * ( w * sin(BRW_theta * 2.0 * r() + bias) + ((1-w) * sin(CRW_theta * 2.0 * r())) ) * y_sign;
	unit_vector();
}


void Dot::unit_vector(){
	int magnitude = sqrt(pow(mVelX, 2) + pow(mVelY, 2))/2;
	if(magnitude != 0){
		mVelX = mVelX/magnitude;
		mVelY = mVelY/magnitude;
	}
}

void Dot::handleEvent( SDL_Event& e ){

	//If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
		random_walks(0, 1, 1);

    }
}

void Dot::move(double w, double bi){
	// randomly switches direction every 100 steps
	if((1 + rand() % 1000) == 2){
		random_walks(w, 1, 1);
	}

	if(bi>0 && bi < 1){
		bias = bi;
		printf("w= %f, bi= %f\n", w, bi);
		random_walks(w,1,1);
	}
	//Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if( mPosX < 0 ){
		random_walks(w, -1, 1);
		mPosX += mVelX;
	}
	else if(mPosX + DOT_WIDTH > SCREEN_WIDTH ){
		random_walks(w, 1, 1);
		mPosX += mVelX;
	}
	
    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( mPosY < 0 ){
		random_walks(w, 1, 1);
		mPosY += mVelY;
	} 
	else if( mPosY + DOT_HEIGHT > SCREEN_HEIGHT ){
		random_walks(w, 1, -1);
		mPosY += mVelY;
	}
    // printf("x: %d, y: %d\n", mPosX, mPosY);
    
    if( mPosX < 400 && mPosX > 350 && mPosY < 100 && mPosY > 50){
        b = 0;
		otw_home = 1;
		mVelX *= -1;
		mVelY *= -1;
    }
    else if(mPosX > 150 && mPosX < 200 && mPosY > 400 && mPosY < 480){
        b = 1;
		otw_home = 0;
		// mVelX *= -1;
		// mVelY *= -1;
    }

}

void Dot::render(){
    //Show the dot
	gAntTexture.render( mPosX, mPosY );
}

int Dot::get_x(){
	return mPosX;
}
int Dot::get_y(){
	return mPosY;
}
int Dot::get_vx(){
	return mVelX;
}
int Dot::get_vy(){
	return mVelY;
}
