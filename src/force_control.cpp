#include "force_control.h"

void PsmForceControl::SetGainsInit()
{
 Mt.diagonal()<<0.1, 0.1, 0.1;


 Kp.diagonal()<<10, 10, 30;
 Kd.diagonal()<<0, 0, 0;

 Cp.diagonal()<<40,40,40;
 Ci.diagonal()<<1,1,1;

}

void PsmForceControl::SetDesiredInit()
{
 x0 = xe;
 q0 = q;

 double incre [3] = {0,0, 0};
 fd << 0, 0, 0;
 xd<< incre[0]+x0(0),incre[1]+x0(1) ,incre[2]+x0(2) ;

 vd << 0, 0, 0;
 ad << 0, 0, 0;

 //ROS_INFO_STREAM("x0: "<< x0<< "   xd: "<< xd);
}


void PsmForceControl::callback_jacobian(std_msgs::Float64MultiArray msg)
{
  
    //Jmin = Ja;

for (int i=0;i<3;i++)
   {
     for (int j=0;j<3;j++)
 {
  Ja(i,j)=msg.data[i*6+j];
     }
  }
  //ROS_INFO_STREAM("Jacobian"<<Ja);

}


void PsmForceControl::callback_joint(sensor_msgs::JointState msg)
{
    for (int i=0;i<3;i++)
    {
    q(i)=msg.position[i];
    qd(i)=msg.velocity[i];
    eff(i)=msg.effort[i];
    }

  //ROS_INFO_STREAM("joint_states"<<q);
}

void PsmForceControl::callback_cartesian(geometry_msgs::PoseStamped msg)
{
    xe(0)=msg.pose.position.x;
    xe(1)=msg.pose.position.y;
    xe(2)=msg.pose.position.z;
    //ROS_INFO_STREAM("cartesian"<<xe);

}

 void PsmForceControl::callback_force(geometry_msgs::Wrench msg)
 {
     he(0) = msg.force.x;
     he(1) = msg.force.y;
     he(2) = msg.force.z;
 }
 void PsmForceControl::CallbackSetForce(geometry_msgs::Pose msg)
 {
    fd(0)=msg.position.x;
    fd(1)=msg.position.y;
    fd(2)=msg.position.z;

 }

 void PsmForceControl::CallbackSetPosition(geometry_msgs::Twist msg)
 {
    xd(0)=msg.linear.x;
    xd(1)=msg.linear.y;
    xd(2)=msg.linear.z;
 }

 void PsmForceControl::CallbackSetPositionIncrement(geometry_msgs::Twist msg)
 {   //The real thangg
     //xd(0)=msg.position.x+xe(0);
     //xd(1)=msg.position.y+xe(1);
     //xd(2)=msg.position.z+xe(2);

     //Just for testing
     xd(0)=msg.linear.x+xd(0);
     xd(1)=msg.linear.y+xd(1);
     xd(2)=msg.linear.z+xd(2);


     //if (q1_traj.check == false){


    /*     q1_traj.qd << xe(0), 0, 0, xd(0),0,0;
         q2_traj.qd << xe(1), 0, 0, xd(1),0,0;
         q3_traj.qd << xe(2), 0, 0, xd(2),0,0;

         q1_traj = interpolate(q1_traj);
         q2_traj = interpolate(q2_traj);
         q3_traj = interpolate(q3_traj);

         //ROS_INFO("IT WORKS");

         t0 = ros::Time::now().toSec();
         t = 0;*/


     //}

     //else{

     //}

 }

 void PsmForceControl::CalcJaM(Eigen::VectorXd q, Eigen::VectorXd qd)
{

    float q1 = q(0);
    float q2 = q(1);
    float q3 = q(2);
    float qd1 = qd(0);
    float qd2 = qd(1);
    float qd3 = qd(2);

// Jacobian from matlab
float t2 = q2-2.908373974667121E-1;
float t3 = cos(t2);
float t4 = 3.141592653589793*(1.0/2.0);
float t5 = -q2+t4+2.908373974667121E-1;
float t6 = q2-t4;
float t7 = sin(t5);
float t8 = cos(t5);
float t9 = sin(t2);
float t10 = cos(t6);
float t11 = t3*t7;
float t12 = t8*t9;
float t13 = t11+t12;
float t14 = t3*t8;
float t17 = t7*t9;
float t15 = t14-t17;
float t16 = sin(t6);
float t18 = q1-t4;
float t19 = sin(t18);
float t20 = t3*t7*t19;
float t21 = t8*t9*t19;
float t22 = t20+t21;
float t23 = t3*t8*t19;
float t25 = t7*t9*t19;
float t24 = t23-t25;
float t26 = q3+4.162E-1;
float t27 = cos(t18);
float t28 = t3*t7*t27;
float t29 = t8*t9*t27;
float t30 = t28+t29;
float t31 = t3*t8*t27;
float t33 = t7*t9*t27;
float t32 = t31-t33;
float t34 = t16*t30;
float t35 = t34-t10*t32;
float t36 = t10*t24;
float t37 = t36-t16*t22;
JaM(0,1) = t3*(3.0/2.0E1)-t10*t13*(4.3E1/1.0E3)+t10*t15*(1.68E2/6.25E2)-t13*t16*(1.68E2/6.25E2)-t15*t16*(4.3E1/1.0E3)-t26*(t10*t15-t13*t16);
JaM(0,2) = -t10*t13-t15*t16;
JaM(1,0) = t3*t19*(-3.0/2.0E1)+t10*t22*(4.3E1/1.0E3)-t10*t24*(1.68E2/6.25E2)+t16*t22*(1.68E2/6.25E2)+t16*t24*(4.3E1/1.0E3)+t26*t37-t3*t8*t19*(1.29E2/2.5E2)+t7*t9*t19*(1.29E2/2.5E2);
JaM(1,1) = t9*t27*(-3.0/2.0E1)-t10*t30*(1.68E2/6.25E2)-t10*t32*(4.3E1/1.0E3)+t16*t30*(4.3E1/1.0E3)-t16*t32*(1.68E2/6.25E2)+t26*(t10*t30+t16*t32);
JaM(1,2) = t35;
JaM(2,0) = t3*t27*(-3.0/2.0E1)+t10*t30*(4.3E1/1.0E3)-t10*t32*(1.68E2/6.25E2)+t16*t30*(1.68E2/6.25E2)+t16*t32*(4.3E1/1.0E3)-t26*t35-t3*t8*t27*(1.29E2/2.5E2)+t7*t9*t27*(1.29E2/2.5E2);
JaM(2,1) = t9*t19*(3.0/2.0E1)+t10*t22*(1.68E2/6.25E2)+t10*t24*(4.3E1/1.0E3)-t16*t22*(4.3E1/1.0E3)+t16*t24*(1.68E2/6.25E2)-t26*(t10*t22+t16*t24);
JaM(2,2) = t37;

}

 void PsmForceControl::CalcN(Eigen::VectorXd q, Eigen::VectorXd qd)
 {
     float q1 = q(0);
     float q2 = q(1);
     float q3 = q(2);
     float qd1 = qd(0);
     float qd2 = qd(1);
     float qd3 = qd(2);

     // N

     float   t2 = cos(q1);
     float t3 = sin(q1);
     float t4 = cos(q2);
     float t5 = cos(2.908373974667121E-1);
     float t6 = sin(q2);
     float t7 = sin(q3);
     float t8 = sin(2.908373974667121E-1);
     float t9 = cos(q3);
     float t10 = qd2*qd2;
     float t11 = q3*2.0;
     float t12 = cos(t11);
     float t13 = q2*2.0;
     float t14 = sin(t11);
     float t15 = qd3*qd3;
     float t16 = cos(t13);
     float t17 = cos(5.816747949334241E-1);
     float t18 = sin(t13);
     float t19 = sin(5.816747949334241E-1);
     float t20 = q3*q3;
     float t21 = qd1*qd1;
     float t22 = t4*t4;
     float t23 = t9*t9;
     float t24 = t5*t5;
     N(0) = q1*6.789509964297665-qd1*4.378325336465812E-2-t2*1.430783703993206E-1+t3*1.028155744757573-((qd1/fabs(qd1)))*2.180323588957116E-1+qd1*qd3*1.128336879962624E-1+t3*t4*8.624819120873413E-1-t3*t5*9.228562676332305-t2*t7*2.460232608209011E-2-t3*t6*1.216989776329687E-1+t2*t9*1.166759863333251E-1-q3*qd1*qd3*1.773863225711729E-2-qd1*qd2*t6*2.825070207040501E-1-qd2*qd3*t4*5.811329365550027E-1+qd1*qd3*t7*2.158979635775254E-4-qd1*qd3*t9*1.023891308639383E-3-qd1*qd3*t12*1.928338689250353E-2+qd1*qd3*t14*2.382719144631059E-1+qd1*qd2*t16*1.51374376407166E-2-qd1*qd2*t18*4.659634886359594E-1-t3*t4*t5*1.038316632917131E-1+t3*t5*t6*1.93980052162195E-1-t3*t4*t8*1.93980052162195E-1-t3*t6*t7*1.166759863333251E-1-t3*t6*t8*1.038316632917131E-1-t3*t6*t9*2.460232608209011E-2+t4*t5*t10*2.641543925360682E-1-t4*t7*t10*9.771986512731708E-3-t5*t6*t10*9.831613760615988E-2+t4*t8*t10*9.831613760615988E-2-t4*t9*t10*1.783841725482498E-1-t6*t7*t10*2.158979635775254E-4+t6*t8*t10*2.641543925360682E-1+t6*t9*t10*1.023891308639383E-3+t4*t7*t15*9.771986512731708E-3+t4*t9*t15*1.783841725482498E-1+t6*t7*t15*2.158979635775254E-4-t6*t10*t12*1.928338689250353E-2-t6*t9*t15*1.023891308639383E-3+t6*t10*t14*2.382719144631059E-1+qd2*qd3*t5*t6*5.06728414408415E-1-qd2*qd3*t4*t8*5.06728414408415E-1-qd1*qd3*t7*t8*7.53132431084391E-4+qd1*qd3*t8*t9*3.571713867346686E-3-qd2*qd3*t4*t12*4.765438289262118E-1-qd2*qd3*t4*t14*3.856677378500706E-2+qd1*qd2*t5*t16*7.450957814263388E-3+qd1*qd2*t4*t19*2.825070207040501E-1+qd1*qd2*t5*t18*5.280501502575559E-2-qd1*qd2*t6*t17*2.825070207040501E-1+qd1*qd2*t7*t16*3.567683450964995E-1-qd1*qd2*t8*t16*5.280501502575559E-2-qd1*qd3*t7*t16*2.158979635775254E-4-qd1*qd2*t7*t18*2.047782617278766E-3-qd1*qd2*t9*t16*1.954397302546342E-2+qd1*qd2*t8*t18*7.450957814263388E-3+qd1*qd3*t7*t18*9.771986512731708E-3+qd1*qd3*t9*t16*1.023891308639383E-3-qd1*qd2*t9*t18*4.317959271550508E-4+qd1*qd3*t9*t18*1.783841725482498E-1-qd1*qd3*t12*t16*1.928338689250353E-2+qd1*qd2*t12*t18*2.382719144631059E-1+qd1*qd3*t14*t16*2.382719144631059E-1+qd1*qd2*t14*t18*1.928338689250353E-2+qd1*qd2*t16*t17*3.519907784220678E-1+qd1*qd3*t16*t17*1.128336879962624E-1-qd1*qd2*t16*t19*8.059462759140663E-1+qd1*qd2*t17*t18*8.059462759140663E-1+qd1*qd3*t16*t19*3.958776574738673E-2-qd1*qd3*t17*t18*3.958776574738673E-2+qd1*qd2*t18*t19*3.519907784220678E-1+qd1*qd3*t18*t19*1.128336879962624E-1+q3*t3*t4*t5*8.69192980598747E-2+q3*t3*t6*t8*8.69192980598747E-2+q3*t4*t5*t10*2.533642072042075E-1+q3*t6*t8*t10*2.533642072042075E-1+t4*t5*t7*t10*7.53132431084391E-4-t4*t5*t9*t10*3.571713867346686E-3-t4*t5*t7*t15*7.53132431084391E-4+t6*t7*t8*t10*7.53132431084391E-4+t4*t5*t9*t15*3.571713867346686E-3-t6*t8*t9*t10*3.571713867346686E-3-t6*t7*t8*t15*7.53132431084391E-4+t6*t8*t9*t15*3.571713867346686E-3-q3*qd1*qd2*t16*t17*7.917553149477345E-2-q3*qd1*qd3*t16*t17*1.773863225711729E-2+q3*qd1*qd2*t16*t19*2.256673759925249E-1-q3*qd1*qd2*t17*t18*2.256673759925249E-1-q3*qd1*qd2*t18*t19*7.917553149477345E-2-q3*qd1*qd3*t18*t19*1.773863225711729E-2+qd1*qd2*t5*t7*t16*7.143427734693371E-3+qd1*qd2*t5*t9*t16*1.506264862168782E-3-qd1*qd3*t5*t7*t18*7.53132431084391E-4+qd1*qd3*t7*t8*t16*7.53132431084391E-4+qd1*qd2*t7*t8*t18*7.143427734693371E-3+qd1*qd3*t5*t9*t18*3.571713867346686E-3-qd1*qd3*t8*t9*t16*3.571713867346686E-3+qd1*qd2*t8*t9*t18*1.506264862168782E-3-qd1*qd2*t16*t19*t20*1.773863225711729E-2+qd1*qd2*t17*t18*t20*1.773863225711729E-2;
     N(1) = q2*(-8.435824140033144E-1)-qd2*1.030517143505567E-1-((qd2/fabs(qd2)))*9.864148220272907E-2+qd2*qd3*2.256673759925249E-1+t2*t4*1.216989776329687E-1+t2*t6*8.624819120873413E-1+t7*t15*1.783841725482498E-1-t9*t15*9.771986512731708E-3+t6*t21*1.412535103520251E-1-t16*t21*7.568718820358301E-3+t18*t21*2.329817443179797E-1-q3*qd2*qd3*3.547726451423457E-2+qd1*qd3*t4*5.811329365550027E-1+qd2*qd3*t7*4.317959271550508E-4-qd2*qd3*t9*2.047782617278766E-3+qd2*qd3*t12*3.856677378500706E-2-qd2*qd3*t14*4.765438289262118E-1-t2*t4*t5*1.93980052162195E-1+t2*t4*t7*1.166759863333251E-1-t2*t5*t6*1.038316632917131E-1+t2*t4*t8*1.038316632917131E-1+t2*t4*t9*2.460232608209011E-2-t2*t6*t8*1.93980052162195E-1+t5*t7*t15*3.571713867346686E-3+t5*t9*t15*7.53132431084391E-4-t5*t16*t21*3.725478907131694E-3-t4*t19*t21*1.412535103520251E-1-t5*t18*t21*2.640250751287779E-2+t6*t17*t21*1.412535103520251E-1-t7*t16*t21*1.783841725482498E-1+t8*t16*t21*2.640250751287779E-2+t7*t18*t21*1.023891308639383E-3+t9*t16*t21*9.771986512731708E-3-t8*t18*t21*3.725478907131694E-3+t9*t18*t21*2.158979635775254E-4-t12*t18*t21*1.191359572315529E-1-t14*t18*t21*9.641693446251764E-3-t16*t17*t21*1.759953892110339E-1+t16*t19*t21*4.029731379570331E-1-t17*t18*t21*4.029731379570331E-1-t18*t19*t21*1.759953892110339E-1+qd1*qd3*t4*t7*2.047782617278766E-3+qd1*qd3*t4*t9*4.317959271550508E-4+qd1*qd3*t6*t7*3.567683450964995E-1-qd1*qd3*t6*t9*1.954397302546342E-2-qd1*qd3*t4*t12*4.765438289262118E-1-qd2*qd3*t7*t8*1.506264862168782E-3-qd1*qd3*t4*t14*3.856677378500706E-2+qd2*qd3*t8*t9*7.143427734693371E-3+q3*t2*t5*t6*8.69192980598747E-2-q3*t2*t4*t8*8.69192980598747E-2+q3*t16*t17*t21*3.958776574738673E-2-q3*t16*t19*t21*1.128336879962624E-1+q3*t17*t18*t21*1.128336879962624E-1+q3*t18*t19*t21*3.958776574738673E-2-t5*t7*t16*t21*3.571713867346686E-3-t5*t9*t16*t21*7.53132431084391E-4-t7*t8*t18*t21*3.571713867346686E-3-t8*t9*t18*t21*7.53132431084391E-4+t16*t19*t20*t21*8.869316128558643E-3-t17*t18*t20*t21*8.869316128558643E-3+qd1*qd3*t5*t6*t7*7.143427734693371E-3-qd1*qd3*t4*t7*t8*7.143427734693371E-3+qd1*qd3*t5*t6*t9*1.506264862168782E-3-qd1*qd3*t4*t8*t9*1.506264862168782E-3;
     N(2) = qd3*(-1.263362987361836)-t10*9.355030110375892E-2-t21*1.128336879962624E-1-((qd3/fabs(qd3)))*3.875510763731035E-1+q3*t10*1.773863225711729E-2+q3*t21*1.773863225711729E-2-t3*t7*1.166759863333251E-1-t3*t9*2.460232608209011E-2-t7*t10*2.158979635775254E-4+t9*t10*1.023891308639383E-3+t10*t14*2.382719144631059E-1-t7*t21*2.158979635775254E-4+t9*t21*1.023891308639383E-3-t10*t23*3.856677378500706E-2-t18*t21*1.979388287369336E-2+t19*t21*1.979388287369336E-2+t21*t22*9.355030110375892E-2+t21*t24*1.128336879962624E-1-qd1*qd2*t4*1.057676765481214-q3*t21*t22*1.773863225711729E-2-q3*t21*t24*1.773863225711729E-2-t2*t4*t5*8.69192980598747E-2-t2*t6*t7*2.460232608209011E-2-t2*t6*t8*8.69192980598747E-2+t2*t6*t9*1.166759863333251E-1+t7*t8*t10*7.53132431084391E-4-t8*t9*t10*3.571713867346686E-3+t7*t8*t21*7.53132431084391E-4-t8*t9*t21*3.571713867346686E-3+t7*t21*t22*2.158979635775254E-4-t9*t21*t22*1.023891308639383E-3+t21*t22*t23*3.856677378500706E-2-t21*t22*t24*2.256673759925249E-1-qd1*qd2*t4*t7*2.047782617278766E-3-qd1*qd2*t4*t9*4.317959271550508E-4-qd1*qd2*t6*t7*3.567683450964995E-1+qd1*qd2*t6*t9*1.954397302546342E-2+qd1*qd2*t4*t23*9.530876578524235E-1+q3*t21*t22*t24*3.547726451423457E-2-t4*t6*t7*t21*9.771986512731708E-3-t4*t6*t9*t21*1.783841725482498E-1+t4*t6*t21*t24*7.917553149477345E-2-t5*t8*t21*t22*7.917553149477345E-2-t7*t8*t21*t22*7.53132431084391E-4-t7*t9*t21*t22*4.765438289262118E-1+t8*t9*t21*t22*3.571713867346686E-3-qd1*qd2*t5*t6*t7*7.143427734693371E-3+qd1*qd2*t4*t7*t8*7.143427734693371E-3+qd1*qd2*t4*t7*t9*7.713354757001412E-2-qd1*qd2*t5*t6*t9*1.506264862168782E-3+qd1*qd2*t4*t8*t9*1.506264862168782E-3+t4*t5*t6*t7*t21*7.53132431084391E-4-t4*t5*t6*t8*t21*2.256673759925249E-1-t4*t5*t6*t9*t21*3.571713867346686E-3+q3*t4*t5*t6*t8*t21*3.547726451423457E-2;

   for (int i=0;i<3;i++)
   {
       if (isnan(N(i))==1)
       {
           N(i)=0;
       }
   }
  // ROS_INFO_STREAM("N: "<<N); //only works if robot is moving
 }


 void PsmForceControl::CalcDiffJacobian(Eigen::VectorXd q, Eigen::VectorXd qd)
 {
    float q1 = q(0);
    float q2 = q(1);
    float q3 = q(2);
    float qd1 = qd(0);
    float qd2 = qd(1);
    float qd3 = qd(2);

    float t2 = 3.141592653589793*(1.0/2.0);
    float t3 = -q2+t2+2.908373974667121E-1;
    float t4 = q2-2.908373974667121E-1;
    float t5 = cos(t4);
    float t6 = cos(t3);
    float t7 = sin(t4);
    float t8 = sin(t3);
    float t9 = q2-t2;
    float t10 = t5*t6;
    float t17 = t7*t8;
    float  t11 = t10-t17;
    float  t12 = sin(t9);
    float  t13 = t5*t8;
    float  t14 = t6*t7;
    float  t15 = t13+t14;
    float  t16 = cos(t9);
    float  t18 = q1-t2;
    float  t19 = sin(t18);
    float  t20 = q3+4.162E-1;
    float  t21 = cos(t18);
    float  t22 = t5*t8*t19;
    float t23 = t6*t7*t19;
    float  t24 = t22+t23;
    float  t25 = t5*t6*t19;
    float  t33 = t7*t8*t19;
    float  t26 = t25-t33;
    float  t27 = qd1*t5*t8*t21;
    float t28 = qd1*t6*t7*t21;
    float  t29 = t27+t28;
    float  t30 = qd1*t5*t6*t21;
    float  t32 = qd1*t7*t8*t21;
    float  t31 = t30-t32;
    float  t34 = qd1*t5*t8*t19;
    float  t35 = qd1*t6*t7*t19;
    float  t36 = t34+t35;
    float  t37 = qd1*t5*t6*t19;
    float  t39 = qd1*t7*t8*t19;
    float  t38 = t37-t39;
    float  t40 = t5*t6*t21;
    float  t45 = t7*t8*t21;
    float  t41 = t40-t45;
    float  t42 = t5*t8*t21;
    float  t43 = t6*t7*t21;
    float  t44 = t42+t43;
    float  t46 = t16*t38;
    float  t47 = qd2*t16*t44;
    float  t48 = qd2*t12*t41;
    float  t49 = t46+t47+t48-t12*t36;
    float  t50 = t12*t29;
    float  t51 = qd2*t16*t24;
    float  t52 = qd2*t12*t26;

     Jd(0,1) = t20*(qd2*t15*cos(q2-3.141592653589793*(1.0/2.0))+qd2*t11*t12)-qd2*t7*(3.0/2.0E1)-qd3*(t11*t16-t12*t15)-qd2*t11*t12*(1.68E2/6.25E2)-qd2*t11*t16*(4.3E1/1.0E3)+qd2*t12*t15*(4.3E1/1.0E3)-qd2*t15*t16*(1.68E2/6.25E2);
     Jd(0,2) = -qd2*t11*t16+qd2*t12*t15;
     Jd(1,0) = t12*t29*(1.68E2/6.25E2)+t12*t31*(4.3E1/1.0E3)+t16*t29*(4.3E1/1.0E3)-t16*t31*(1.68E2/6.25E2)-qd3*(t12*t24-t16*t26)-t20*(t50+t51+t52-t16*t31)-qd1*t5*t21*(3.0/2.0E1)+qd2*t7*t19*(3.0/2.0E1)-qd2*t12*t24*(4.3E1/1.0E3)+qd2*t12*t26*(1.68E2/6.25E2)+qd2*t16*t24*(1.68E2/6.25E2)+qd2*t16*t26*(4.3E1/1.0E3)-qd1*t5*t6*t21*(1.29E2/2.5E2)+qd1*t7*t8*t21*(1.29E2/2.5E2);
     Jd(1,1) = -t20*(t12*t38+t16*t36+qd2*t12*t44-qd2*t16*t41)-t12*t36*(4.3E1/1.0E3)+t12*t38*(1.68E2/6.25E2)+t16*t36*(1.68E2/6.25E2)+t16*t38*(4.3E1/1.0E3)+qd3*(t12*t41+t16*t44)+qd1*t7*t19*(3.0/2.0E1)-qd2*t5*t21*(3.0/2.0E1)+qd2*t12*t41*(4.3E1/1.0E3)+qd2*t12*t44*(1.68E2/6.25E2)-qd2*t16*t41*(1.68E2/6.25E2)+qd2*t16*t44*(4.3E1/1.0E3);
     Jd(1,2) = t49;
     Jd(2,0) = t12*t36*(-1.68E2/6.25E2)-t12*t38*(4.3E1/1.0E3)-t16*t36*(4.3E1/1.0E3)+t16*t38*(1.68E2/6.25E2)-t20*t49-qd3*(t12*t44-t16*t41)+qd1*t5*t19*(3.0/2.0E1)+qd2*t7*t21*(3.0/2.0E1)+qd2*t12*t41*(1.68E2/6.25E2)-qd2*t12*t44*(4.3E1/1.0E3)+qd2*t16*t41*(4.3E1/1.0E3)+qd2*t16*t44*(1.68E2/6.25E2)+qd1*t5*t6*t19*(1.29E2/2.5E2)-qd1*t7*t8*t19*(1.29E2/2.5E2);
     Jd(2,1) = -t20*(t12*t31+t16*t29-qd2*t12*t24+qd2*t16*t26)-t12*t29*(4.3E1/1.0E3)+t12*t31*(1.68E2/6.25E2)+t16*t29*(1.68E2/6.25E2)+t16*t31*(4.3E1/1.0E3)-qd3*(t12*t26+t16*t24)+qd2*t5*t19*(3.0/2.0E1)+qd1*t7*t21*(3.0/2.0E1)-qd2*t12*t24*(1.68E2/6.25E2)-qd2*t12*t26*(4.3E1/1.0E3)-qd2*t16*t24*(4.3E1/1.0E3)+qd2*t16*t26*(1.68E2/6.25E2);
     Jd(2,2) = -t50-t51-t52+t16*t31;

    // ROS_INFO_STREAM("Jd: "<<Jd);

 }
void PsmForceControl::CalcM(Eigen::VectorXd q) //Eigen::VectorXd qd)
{

    float q1 = q(0);
    float q2 = q(1);
    float q3 = q(2);
    //float qd1 = qd(1);
    //float qd2 = qd(2);
    //float qd3 = qd(3);

    float t2 = q2*2.0;
    float t3 = q3*2.0;
    float t4 = cos(q2);
    float t5 = cos(q3);
    float t6 = sin(2.908373974667121E-1);
    float t7 = sin(5.816747949334241E-1);
    float t8 = sin(q3);
    float t9 = cos(t2);
    float t10 = cos(2.908373974667121E-1);
    float t11 = cos(5.816747949334241E-1);
    float t12 = sin(t2);
    float t13 = cos(t3);
    float t14 = sin(t3);
    float t15 = q3*q3;
    float t16 = sin(q2);
    float t17 = t4*t5*5.119456543196916E-4;
    float t18 = t5*t16*8.919208627412488E-2;
    float t19 = t4*t6*1.320771962680341E-1;
    float t20 = t8*t16*4.885993256365854E-3;
    float t21 = t4*t14*1.191359572315529E-1;
    float t22 = t4*t6*t8*3.765662155421955E-4;
    float t23 = q3*t4*t6*1.266821036021037E-1;
    float t24 = t5*t10*t16*1.785856933673343E-3;
    float t25 = t17+t18+t19+t20+t21+t22+t23+t24-t4*t8*1.079489817887627E-4-t4*t10*4.915806880307994E-2-t4*t13*9.641693446251764E-3-t6*t16*4.915806880307994E-2-t10*t16*1.320771962680341E-1-q3*t10*t16*1.266821036021037E-1-t4*t5*t6*1.785856933673343E-3-t8*t10*t16*3.765662155421955E-4+1.374393583700811E-2;
    float t26 = t16*2.905664682775014E-1;
    float t27 = t4*t5*4.885993256365854E-3;
    float t28 = t4*t10*1.266821036021037E-1;
    float t29 = t5*t16*1.079489817887627E-4;
    float t30 = t8*t16*5.119456543196916E-4;
    float t31 = t6*t16*1.266821036021037E-1;
    float t32 = t26+t27+t28+t29+t30+t31-t4*t8*8.919208627412488E-2-t4*t5*t10*3.765662155421955E-4-t4*t8*t10*1.785856933673343E-3-t5*t6*t16*3.765662155421955E-4-t6*t8*t16*1.785856933673343E-3;
    float t33 = t5*8.919208627412488E-2;
    float t34 = t8*4.885993256365854E-3;
    float t35 = t5*t10*1.785856933673343E-3;
    float t36 = t33+t34+t35-t8*t10*3.765662155421955E-4+1.979388287369336E-2;


    M(0,0) = q3*(-5.641684399813122E-2)-t4*1.412535103520251E-1+t5*1.079489817887627E-4-t6*1.862739453565847E-3+t7*7.206792799878022E-2+t8*5.119456543196916E-4-t9*1.164908721589898E-1+t10*1.32012537564389E-2-t12*3.78435941017915E-3+t13*5.956797861577647E-2+t14*4.820846723125882E-3+t15*4.434658064279322E-3-t5*t6*3.765662155421955E-4-t5*t9*1.079489817887627E-4-t6*t8*1.785856933673343E-3-t4*t11*1.412535103520251E-1+t6*t9*1.862739453565847E-3+t7*t9*8.799769460551694E-2+t5*t12*4.885993256365854E-3-t8*t9*5.119456543196916E-4+t6*t12*1.32012537564389E-2+t7*t12*2.014865689785166E-1+t9*t10*1.32012537564389E-2-t8*t12*8.919208627412488E-2+t9*t11*2.014865689785166E-1+t9*t13*5.956797861577647E-2-t10*t12*1.862739453565847E-3-t7*t16*1.412535103520251E-1+t9*t14*4.820846723125882E-3-t11*t12*8.799769460551694E-2-q3*t7*t9*1.979388287369336E-2-q3*t7*t12*5.641684399813122E-2-q3*t9*t11*5.641684399813122E-2+q3*t11*t12*1.979388287369336E-2+t5*t6*t9*3.765662155421955E-4+t6*t8*t9*1.785856933673343E-3-t5*t10*t12*3.765662155421955E-4-t8*t10*t12*1.785856933673343E-3+t7*t12*t15*4.434658064279322E-3+t9*t11*t15*4.434658064279322E-3+3.761269948597414E-1;
    M(0,1) = t25;
    M(0,2) = t32;
    M(1,0) = t25;
    M(1,1) = q3*(-1.128336879962624E-1)+t5*2.158979635775254E-4-t6*3.725478907131694E-3+t8*1.023891308639383E-3+t10*2.640250751287779E-2-t13*1.191359572315529E-1-t14*9.641693446251764E-3+t15*8.869316128558643E-3-t5*t6*7.53132431084391E-4-t6*t8*3.571713867346686E-3+5.741764099066974E-1;
    M(1,2) = t36;
    M(2,0) = t32;
    M(2,1) = t36;
    M(2,2) = 2.9943578440606E-1;

    //ROS_INFO_STREAM("M: "<< M);
}

void PsmForceControl::CalcU()
 {    // This is parallel/position/force

//     xf = Cp*(fd-he);
//     ve = Ja*qd;
//     y = Ja.inverse()*M.inverse()*(-Kd*xe_v+Kp*((xd-xe)+xf)-M*Jd*qd);
//     u = M*y+N+Ja.transpose()*he;
      //this->interpolate();

    ve = Ja*qd;
    ROS_INFO_STREAM("x increment: "<< xd - xe);

    if (q1_traj.check==true){


        x_int << q1_traj.x(t),q2_traj.x(t),q3_traj.x(t);
        v_int << q1_traj.v(t),q2_traj.v(t),q3_traj.v(t);
        a_int << q1_traj.a(t),q2_traj.a(t),q3_traj.a(t);

        //y = Ja.inverse()*Mt.inverse()*(Mt*a_int+Kd*(v_int-ve)+Kp*(x_int-xe)-Mt*Jd*qd-he);

        y = Ja.inverse()*Mt.inverse()*(Mt*a_int+Kd*(v_int-ve)+Kp*(x_int-xe)-he);
        u = M*y + Ja.transpose()*he;

        //ROS_INFO_STREAM("v_int at "<< t << " : "<< v_int);

        t = t + 1;
        ROS_INFO_STREAM("u_int at "<< t << " : "<< u);

        if (t == rate*tf)
        {
            t = 0;
            q1_traj.check = false;
            q2_traj.check = false;
            q3_traj.check = false;
        }
    }
    else{

       //y = Ja.inverse()*Mt.inverse()*(Mt*ad+Kd*(vd-ve)+Kp*(xd-xe)-Mt*Jd*qd-he);

        y = Ja.inverse()*M.inverse()*(M*ad+Kd*(vd-ve)+Kp*(xd-xe)-he);
        u = M*y + Ja.transpose()*he;

        //u = M*(xd-xe);

        ROS_INFO_STREAM("u_steady : "<< u);
    }


       //ROS_INFO_STREAM("\nu: "<< u);
       //ROS_INFO_STREAM("x increment: "<< xd - xe);

        ROS_INFO_STREAM("Check Direction: "<< Ja.inverse()*(xd-xe)*1000);


    //ROS_INFO_STREAM("M: "<< M);

      // ROS_INFO_STREAM("Ja: "<< Ja);
       //ROS_INFO_STREAM("JaM: "<< JaM);

   // ROS_INFO_STREAM("Ja inverse: "<< JaM.inverse());
   // ROS_INFO_STREAM("Ja inverse: "<< Ja.inverse());
       //ROS_INFO_STREAM("M inverse: "<< M.inverse());
       //ROS_INFO_STREAM("Move Coef: "<< Ja.inverse()*Mt.inverse());

      // ROS_INFO_STREAM("All Move Coef: "<< M*Ja.inverse()*Mt.inverse());
       //ROS_INFO_STREAM("N: "<< N);
       //ROS_INFO_STREAM("ve: "<< ve);

       //ROS_INFO_STREAM("he: "<<(M*ad+Kd*(vd-ve)+Kp*(xd-xe)-M*Jd*qd-he));
       //ROS_INFO_STREAM("KP: "<<Kp*(xd-xe));
       //ROS_INFO_STREAM("Kd: "<<Kd*(vd-ve));
       //ROS_INFO_STREAM("ve: "<< ve<< "vd"<< vd << "Kd: "<<Kd );
 }

void PsmForceControl::Interpolate(Eigen::VectorXd x0, Eigen::VectorXd xf)
{
 int a = 0 ;
}

void PsmForceControl::output()
 {
   std::cout<<u;

  joint_msg.effort[0]=u(0);
  joint_msg.effort[1]=u(1);
  joint_msg.effort[2]=u(2);


  //joint_pub.publish(joint_msg);


  /* msg2.velocity[0] = qd(0);
   msg2.velocity[1] = qd(1);
   msg2.velocity[2] = qd(2);

   plot_x.publish(msg2);*/

  msg1.data = qd(0);
  plot_y.publish(msg1);
 }

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "PsmForceControl_node");
  ros::NodeHandle n;
 
  std_msgs::String mes;
  std::stringstream ss;
  ss<<"it starteed";
  mes.data = ss.str();
  ROS_INFO("%s", mes.data.c_str());
  std::cout<<123;

  PsmForceControl obj(n);
  // int i, j;
  int count = 0;
  ros::Duration(0.1).sleep();
  ros::spinOnce();
  obj.SetGainsInit();
  obj.SetDesiredInit();

  std_msgs::Float64 msg;
  // char link[100];
    //Interpolate a;




  ros::Rate r(obj.rate);
    ROS_INFO("%s", mes.data.c_str());
  while(ros::ok())
  {
  obj.CalcJaM(obj.q,obj.qd);
  obj.CalcN(obj.q,obj.qd);

  obj.CalcDiffJacobian(obj.q,obj.qd);

  obj.CalcM(obj.q);

  obj.CalcU();
  obj.output();
  //count=count+1;
  r.sleep();
  ros::spinOnce();


  }
ros::spin();

}
