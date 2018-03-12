#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <string>
#include <vector>
#include "Movie.hpp"
#include "GraphEdge.hpp"
#include <climits>
#include <utility>
using namespace std;

class Actor {
	public:
		Actor(string actorName):name(actorName) {}
		string name;
		vector<GraphEdge*> friends;
		int dist;
		GraphEdge* prev; //Point to graph edge
		bool visited = false;

		
		/*void addFriend(void* movie) {
			int movieWeight = movie->weight;
			GraphEdge* current;
			auto it = movie->actors.begin();
			auto itE = movie->actors.end();
			while(it != itE) {
				current = new GraphEdge(this->name, (*it)->name, movie, movieWeight);
				friends.push_back(current);
				(*it)->addEdge(current);
				it++;
			}
		}
	*/
		void addEdge(GraphEdge* edge) {
			friends.push_back(edge);
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
			this->dist = INT_MAX;
			this->prev = nullptr; //Point to a GraphEdge
		}

};


#endif
