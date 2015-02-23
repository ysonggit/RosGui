#ifndef POSE_H
#define POSE_H

struct Pose{
    std::string x;
    std::string y;
    std::string theta;
Pose():x(""), y(""), theta(""){}
Pose(std::string _x, std::string _y, std::string _t) : x(_x), y(_y), theta(_t){}
};

#endif