//
// Created by radian on 6/29/18.
//

#include "psm_coop/Cooperative.h"

int main(int argc, char **argv)
{
    string choose[2];
    ros::init(argc, argv, "PsmForceControl_node");
    ros::NodeHandle n;

    choose[0] = "Cartesian";
    choose[1] = "Impedance";

    ros::init(argc, argv, "PsmForceControl_node");

    int num = 1;

    initializer psm[2];
    //psm[0]= psm1; psm[1] = psm2;


    psm[0].name = "PSM1";
    psm[0].type = "Master";
    psm[0].ctrl_type = choose[1];
    psm[0].Rot.resize(3,3);
    psm[0].Rot << 1, 0, 0,
            0, 1, 0,
            0, 0, 1;

    psm[0].Pos.resize(3);
    psm[0].Pos << 0, 0, 0;

    psm[1].name = "PSM2";
    psm[1].type = "Master";
    psm[1].ctrl_type = choose[0];

    psm[1].Rot.resize(3,3);
    psm[1].Rot << 1, 0, 0,
            0, 1, 0,
            0, 0, 1;

    psm[1].Pos.resize(3);
    psm[1].Pos << 2, 2, 2;


    std::vector<initializer> m;

    m.push_back(psm[0]);
    m.push_back(psm[1]);

    Cooperative obj(m, n);
    obj.CalcObject();
    
}