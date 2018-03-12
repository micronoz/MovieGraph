#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include "Actor.hpp"

using namespace std;

class Movie {
	public:	
		int weight;
		Movie(string nameIn, int yearIn, bool weighted):name(nameIn), year(yearIn) {
			if(weighted)
				this->weight = 1+(2018-yearIn);
			else
				this->weight = 1;
		}
		void addActor(Actor* actorName) {
			actors.push_back(actorName);
		}

		string getMovie() {
			return this->name;
		}
		

		
		vector<Actor*> actors;
		string name;
		int year;
	protected:
		bool visited = false;


};
#endif
