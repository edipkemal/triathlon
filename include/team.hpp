#ifndef TEAM_HPP
#define TEAM_HPP

#include <vector>
#include "athlete.hpp"

class Team {
private:
    int number; // Team number
    float totalPoints; // Total points of the team
    std::vector<Athlete> members; // Vector to store team members

public:
    // Constructor
    Team(int number);

    // Function to add an athlete to the team
    void addMember(Athlete& athlete);

    // Function to add points to the team
    void addPoints(float points);

    // Getters
    int getNumber() const;
    float getTotalPoints() const;
    std::vector<Athlete>& getMembers();
};

#endif // TEAM_HPP
