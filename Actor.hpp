#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include "Movie.hpp"
using namespace std;

class Actor {
	public:
		Actor(string actorName):name(actorName) {}
		string name;
		vector<string> movies;
		bool visited = false;

		void addMovie(string movie) {
			movies.push_back(movie);
		}

		bool isVisited() {
			return this->visited;
		}

		void visit() {
			this->visited = true;
		}
	
		void devisit() {
			this->visited = false;
		}

};


#endif
