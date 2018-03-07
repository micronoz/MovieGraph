#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include "Movie.hpp"
#include <utility>
using namespace std;

class Actor {
	public:
		Actor(string actorName):name(actorName) {}
		string name;
		vector<string> movies;
		vector<Actor*> friends;
		int dist;
		pair<Actor*, string> prev;
		bool visited = false;

		void addMovie(string movie) {
			movies.push_back(movie);
		}
		void addFriend(Actor* actorName) {
			friends.push_back(actorName);
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

		void reset() {
			this->devisit();
			this->dist = 0;
			this->prev = pair<Actor*, string>(nullptr, "");
		}

};


#endif
