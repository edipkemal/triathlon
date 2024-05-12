#include "../include/team.hpp"

// Constructor
Team::Team(int number) : number(number), totalPoints(0) {}

// Function to add an athlete to the team
void Team::addMember(Athlete& athlete)
{
    members.push_back(athlete);
}

// Function to add points to the team
void Team::addPoints(float points) {
    totalPoints += points;
}

// Getters
int Team::getNumber() const {
    return number;
}

float Team::getTotalPoints() const {
    return totalPoints;
}

std::vector<Athlete>& Team::getMembers()
{
    return members;
}
