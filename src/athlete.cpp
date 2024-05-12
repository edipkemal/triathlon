#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

#include "../include/athlete.hpp"

// Constructor
Athlete::Athlete(int number, int teamNumber, float firstPass, float secondPass, float finishLine) : number(number), teamNumber(teamNumber), firstPass(firstPass), secondPass(secondPass), finishLine(finishLine)
{
    waitTime = 0;
    position = 0;

    // Initialize speed with a random value between 1.0 and 5.0
    speed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 4.0f) + 1.0f;
    initialSpeed = speed;
    track = 1;
    time = 0;
    finishTime = 0;
    visible = 0;
}

void Athlete::updateTrack(int newTrack)
{
    track = newTrack;
    waitTime = 10;

    if (track == 2)
    {
        speed = initialSpeed * 3;
    }
    else if (track == 3)
    {
        speed = initialSpeed / 3;
    }
    else
    {
        speed = initialSpeed;
    }
}

void Athlete::updatePosition()
{
    if (waitTime > 0)
    {
        time++;
        waitTime--;
    }
    else
    {
        if (position >= secondPass && track == 2)
        {
            updateTrack(3);
        }
        else if (position >= firstPass && track == 1)
        {
            updateTrack(2);
        }
        else if (position >= finishLine)
        {
            finishTime = time;
        }
        else
        {
            time++;
            position += speed;
        }
    }
    if (visible)
    {
        std::cout << "Team: " << teamNumber << ", Athlete: " << number
                  << ", Position: " << position << " meters, Speed: " << speed << " m/s" << std::endl;
    }
}

void Athlete::setSpeed(float newSpeed)
{
    speed = newSpeed;
}

float Athlete::getSpeed()
{
    return speed;
}

float Athlete::getPosition() const
{
    return position;
}

int Athlete::getNumber() const
{
    return number;
}

int Athlete::getTeamNumber() const
{
    return teamNumber;
}

int Athlete::getFinishTime()
{
    return finishTime;
}

void Athlete::setFinishTime(int newFinishTime)
{
    finishTime = newFinishTime;
}

void Athlete::setTime(int newTime)
{
    time = newTime;
}

int Athlete::getTime() const
{
    return time;
}

bool Athlete::operator<(const Athlete& other) const 
{
    return finishTime == other.finishTime ? position > other.position : finishTime < other.finishTime;
}