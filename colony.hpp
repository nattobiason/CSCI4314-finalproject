#include "Dot.hpp"


int N = 0;

class Colony{
    public:
        Colony(int);
        std::vector <Dot> ants;
        std::vector<Pheromone> p_trail;
        int num_ants;
        void move();
        void render();
        void render_trail();
        double p_bias(int, int, bool);
        double W(double, int, double);
        float pheromone_exists(int, int, bool);
    private:
        double theta0[3];

};


Colony::Colony(int n){
    num_ants = n;
    for(int i = 0; i < n; i++){
        ants.push_back(Dot());
    }
    theta0[0] = 5* pi/6;
    theta0[1] = pi/2; 
    theta0[2] = pi/6;
}

void Colony::move(){

    for(int i = 0; i < num_ants; i++){
        // if(p_bias() > 0) ants[i].biased_random_walks();
        double u = p_bias(ants[i].get_x(), ants[i].get_y(), ants[i].otw_home);
		ants[i].move(u, W(u, 4, 0.2));
        // if(u > 0.0) ants[i].move(u, W(u, 4, 0.2));
        // else ants[i].move(0, 0);
        if(N%10==0){
		    p_trail.push_back( Pheromone(ants[i].get_x(), ants[i].get_y(), 1.0, ants[i].b) );
        }
    }
    N++;
}

void Colony::render(){
    render_trail();
    for(int i = 0; i < num_ants; i++){
        ants[i].render();
    }

    gHomeTexture.render( 150, 400 );
	gHomeTexture.render( 200, 480 );
	gFoodTexture.render( 350, 50 );
}

void Colony::render_trail(){
    int size = p_trail.size();
    
    for(int i = 0; i < size; i++){
        p_trail[i].render_p();
		if(p_trail[i].update_concentration() <= 0.00001){
			p_trail.erase (p_trail.begin()+i);
		}
    }
}

float Colony::pheromone_exists(int x, int y, bool b){
	int size = p_trail.size();
	for(int i = 0; i < size; i++){
		if(p_trail[i].mPosX == x && p_trail[i].mPosY == y && !(p_trail[i].home_trail ^ b)){ // !(p_trail.home_trail ^ b) checks if pheromone is the one the ant is looking for
			printf("here");
			return p_trail[i].update_concentration();
		}
	}
	return 0.0;
}

double Colony::W(double sigma, int beta = 4, double delta = 0.2){
	return pow(1 + sigma/(1 + (delta*sigma)), beta);
}

// returns bias direction
double Colony::p_bias(int PosX, int PosY, bool b){
    /*  LEFT BOUNDRY  */
	double w_left = 0.0;

	int x = 0 - (20 * sqrt(2)/2);
	int tan_constant = tan((7* pi)/18);
	int sqrt_constant = 1/(sqrt(3));

	int ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
	int lambda_boundry = sqrt(0-pow(x, 2) + 400);
	int right_boundry = (0-tan_constant) * x;
	int left_boundry = 0-(sqrt_constant * x);

	int y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
	int lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;

	while(x <= -2){
		while(y >= lower_boundry){
			double d = pheromone_exists(PosX + x, PosY + y, b);
			if(d > 0) w_left += d;
			y--;
		}
		x++;
		ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
		lambda_boundry = sqrt(0-pow(x, 2) + 400);
		right_boundry = (0-tan_constant) * x;
		left_boundry = 0-(sqrt_constant * x);
		y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
		lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;
	}





    /*  MIDDLE BOUNDRY  */
	double w_mid = 0.0;

	x = -5;

	ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
	lambda_boundry = sqrt(0-pow(x, 2) + 400);

	right_boundry = tan_constant * x;
	left_boundry = (0-tan_constant) * x;

	y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
	lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;

	while(x <= 5){
		while(y >= lower_boundry){
			double d = pheromone_exists(PosX + x, PosY + y, b);
			if(d > 0) w_mid += d;
			y--;
		}
		x++;
		ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
		lambda_boundry = sqrt(0-pow(x, 2) + 400);
		right_boundry = tan_constant * x;
	    left_boundry = (0-tan_constant) * x;
		y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
		lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;
	}






    /*  RIGHT BOUNDRY  */
	double w_right = 0.0;

	x = 3;

	ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
	lambda_boundry = sqrt(0-pow(x, 2) + 400);

	right_boundry = x * sqrt_constant;
	left_boundry = tan_constant * x;

	y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
	lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;

	while(x <= 8){
		while(y >= lower_boundry){
			double d = pheromone_exists(PosX + x, PosY + y, b);
			if(d > 0) w_right += d;
			y--;
		}
		x++;
		ant_edge = sqrt(0-pow(x, 2) + pow(20/2, 2));
		lambda_boundry = sqrt(0-pow(x, 2) + 400);
		right_boundry = x * sqrt_constant;
	    left_boundry = tan_constant * x;
		y = (lambda_boundry <= right_boundry) ? lambda_boundry : right_boundry;
		lower_boundry = (left_boundry >= ant_edge) ? left_boundry : ant_edge;
	}

	if(w_left <= 0 && w_right <= 0 && w_mid <= 0) return 0.0;
    if(w_left >= w_mid){
        return (w_left >= w_right) ? theta0[0] : theta0[1];
    }
    else if(w_mid >= w_right){
        return theta0[2];
    }
	return 0;

}