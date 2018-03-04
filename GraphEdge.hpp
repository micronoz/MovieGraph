#ifndef Movie_HPP
#define Movie_HPP

#include <vector>
#include <string>

class Movie {
	public:
		Movie(string nameIn, int yearIn):name(nameIn), year(yearIn) {}
	
		void addActor(string actorName) {
			actors.push_back(actorName);
		}
	protected:
		string name;
		int year;
		vector<string> actors;


}


#endif
