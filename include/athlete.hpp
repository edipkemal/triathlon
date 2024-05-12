#ifndef ATHLETE_HPP
#define ATHLETE_HPP

class Athlete {
private:
    float firstPass;
    float secondPass;
    float finishLine;
    int number;         // Athlete number
    int teamNumber;     // Team number
    float position;     // Current position
    float speed;        // Current speed
    float initialSpeed; // Initial speed
    int waitTime;       // Waiting time for parkour change
    int track;          // Current track
    int time;           // Current time
    int finishTime;     // Finish time
    bool visible;
public:
    // Constructor
    Athlete(int number, int teamNumber, float firstPass, float secondPass, float finishLine);

    //Function to update track acording to position
    void updateTrack(int newTrack);

    // Function to update athlete's position based on speed
    void updatePosition();

    // Function to set athlete's speed
    void setSpeed(float newSpeed);

    // Function to get athlete's speed
    float getSpeed();

    // Function to get athlete's position
    float getPosition() const;

    // Function to get athlete's number
    int getNumber() const;

    // Function to get athlete's team number
    int getTeamNumber() const;

    // Function to get finish time
    int getFinishTime();

    void setFinishTime(int newFinishTime);
    void setTime(int newTime);
    int getTime() const;

    bool operator<(const Athlete& other) const;

    void setVisible(bool value) { visible = value; }
    bool isVisible() const { return visible; }
};

#endif // ATHLETE_HPP
