/*
    Created by Edip Kemal Sardoğan May 2024
*/
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm> 
#include <chrono>  
#include <mutex>     
#include "../include/athlete.hpp"
#include "../include/team.hpp"

const int totalTeams = 300;
const int simuleteSecond = 1000; // A second in milliseconds

// Track distances in meters
float firstPass = 5000.0f;
float secondPass = firstPass + 40000.0f;
float finishLine = secondPass + 10000.0f;

bool showInfo = false;
// Function to update positions of athletes in a team
void updatePositions(Team& team, std::mutex& mutex)
{
    std::lock_guard<std::mutex> lock(mutex);

    // Lock the mutex to ensure exclusive access to team's data
    

    // Loop through each athlete in the team
    for (Athlete& athlete : team.getMembers())
    {
        // Update athlete's position based on their speed
        athlete.updatePosition();
    }
}

// Function to create a team
Team createTeam(int teamNumber)
{
    Team team(teamNumber);
    for (int i = 1; i <= 3; ++i)
    {
        Athlete athlete(i, teamNumber, firstPass, secondPass, finishLine);
        team.addMember(athlete);
    }
    return team;
}

// Function to simulate the race
void simulateRace(std::vector<Team>& teams)
{
    Athlete* winnerAthlete = nullptr;
    Team* winnerTeam = nullptr;

    bool raceFinished = false;
    bool positionsPrinted = false; // Flag to check if positions are printed after first athlete finishes

    constexpr int numThreads = 10; // Number of threads to use
    const int teamsPerThread = teams.size() / numThreads; // Number of teams to handle per thread

    std::vector<std::thread> threads; // Vector to store threads
    std::mutex mutex;                  // Mutex for synchronizing access to team data

    while (!raceFinished)
    {
        if (showInfo)
        {
            std::cout << std::endl << "------------------------------------------------------------" << std::endl;
        }
        // Create and start threads to update positions for each group of teams
        for (int i = 0; i < numThreads; ++i)
        {
            int startIndex = i * teamsPerThread;
            int endIndex = (i == numThreads - 1) ? teams.size() : (startIndex + teamsPerThread);

            threads.push_back(std::thread([&teams, &mutex, startIndex, endIndex]() {
                for (int j = startIndex; j < endIndex; ++j)
                {
                    updatePositions(teams[j], mutex);
                }
            }));
        }

        // Join all threads to wait for them to finish
        for (std::thread& thread : threads)
        {
            thread.join();
        }
        threads.clear(); // Clear the vector for next iteration

        // Check if any athlete has finished the race
        bool anyAthleteFinished = false;
        for (Team& team : teams)
        {
            for (Athlete& athlete : team.getMembers())
            {
                if (athlete.getFinishTime() > 0)
                {
                    anyAthleteFinished = true;
                    break;
                }
            }
            if (anyAthleteFinished)
            {
                break;
            }
        }

        // Print current positions if any athlete has finished and positions are not printed yet
        if (anyAthleteFinished && !positionsPrinted)
        {
            std::cout << "Current positions at the moment of first athlete reaching the finish line:" << std::endl;
            std::vector<Athlete> allAthletes;
            for (Team& team : teams)
            {
                for (Athlete& athlete : team.getMembers())
                {
                    allAthletes.push_back(athlete);
                }
            }
            std::sort(allAthletes.begin(), allAthletes.end(), [](Athlete& a, Athlete& b) {
                return a.getPosition() > b.getPosition();
            });

            for (int i = 0; i < allAthletes.size(); ++i)
            {
                Athlete& athlete = allAthletes[i];
                if (i == 0)
                {
                    winnerAthlete = &athlete;
                    std::cout << "---Finish time: " << athlete.getFinishTime() << " seconds---" << std::endl;
                }
                std::cout << "Rank " << i + 1 << ": Team " << athlete.getTeamNumber() << ", Athlete " << athlete.getNumber()
                          << " - Position: " << athlete.getPosition() << " meters, Speed: " << athlete.getSpeed() << " m/s" << std::endl;
            }
            std::cout << std::endl;
            positionsPrinted = true; // Set flag to true to indicate positions are printed
        }

        // Check if all athletes have finished the race
        raceFinished = true;
        for (Team& team : teams)
        {
            for (Athlete& athlete : team.getMembers())
            {
                if (athlete.getFinishTime() == 0)
                {
                    raceFinished = false;
                    break;
                }
            }
            if (!raceFinished)
            {
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(simuleteSecond));
    }

    std::sort(teams.begin(), teams.end(), [](Team& a, Team& b) {
        int totalTimeA = 0, totalTimeB = 0;
        for (Athlete& athlete : a.getMembers())
        {
            totalTimeA += athlete.getFinishTime();
            a.addPoints(athlete.getFinishTime());
        }
        for (Athlete& athlete : b.getMembers())
        {
            totalTimeB += athlete.getFinishTime();
            b.addPoints(athlete.getFinishTime());

        }
        return totalTimeA < totalTimeB;
    });

    // Print information of teams
    std::cout << "Teams" << std::endl;
    int totalTime = 0;

    for (int i = 0; i < totalTeams; ++i)
    {
        Team& team = teams[i];
        if (i == 0)
        {
            winnerTeam = &team;
        }
        totalTime = team.getTotalPoints();
        std::cout << "Rank " << i + 1 << ": Team " << team.getNumber() << " - Total Finish Time: " << totalTime << " seconds" << std::endl;
    }

    std::cout  << "------------------------------------------------------------" << std::endl;

    std::cout << "*** Winner Athlete: number " << winnerAthlete->getNumber() << " of the team " << winnerAthlete->getTeamNumber() << std::endl;

    std::cout << "*** Winner Team: number " << winnerTeam->getNumber() << std::endl;

}

int main()
{

    std::vector<Team> teams;

    // Create teams
    for (int i = 1; i <= totalTeams; ++i)
    {
        teams.push_back(createTeam(i));
    }

    // Take user input for athlete visibility
    int teamNumber, athleteNumber;
    std::cout << "Enter the team number (1-300) and athlete number (1-3) to set it visible, or enter 0 0 to start the race:" << std::endl;
    while (true)
    {
        std::cin >> teamNumber >> athleteNumber;
        if (teamNumber == 0 && athleteNumber == 0)
        {
            break; // Start race
        }
        if (teamNumber < 1 || teamNumber > totalTeams || athleteNumber < 1 || athleteNumber > 3)
        {
            std::cout << "Invalid input. Please enter valid team number (1-300) and athlete number (1-3)." << std::endl;
            continue;
        }
        // Set visibility for the athlete
        teams[teamNumber - 1].getMembers()[athleteNumber - 1].setVisible(true);
        std::cout << "Athlete " << athleteNumber << " of Team " << teamNumber << " is now visible." << std::endl;
        showInfo = true;
    }

    // Simulate the race
    simulateRace(teams);

    return 0;
}
