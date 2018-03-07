#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <vector>
#include <string>
#include "Actor.hpp"

using namespace std;

class Movie {
	public:
		Movie(string nameIn, int yearIn, bool weighted):name(nameIn), year(yearIn) {
			if(weighted)
				this->weight = 1+(2018-yearIn);
		}
		int weight = 1;
		void addActor(Actor* actorName) {
			actors.push_back(actorName);
		}

		string getMovie() {
			return this->name;
		}
		
		vector<Actor*>& getActors() {
			return actors;
		}

		
	protected:
		string name;
		int year;
		vector<Actor*> actors;
		bool visited = false;


};
#endif
