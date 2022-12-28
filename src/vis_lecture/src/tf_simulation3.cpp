#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <math.h>

#define theta_to_rad(theta) theta*0.0174533
#define rad_to_theta(rad) rad*57.2957805
#define pi                  3.1415926   // rad，圓周率
#define wheel_radius        0.05        // m，輪子半徑
#define wheel_distance      0.25        // m，兩輪間距
#define wheel_circumference 0.31415926  // m，輪子周長
#define Center_Velocity     0.1        // m/s，中心速度
#define Angular_Velocity    0.5        // rad/s，角速度
#define sleep_hz            100.0      // hz/s，node工作頻率

struct pose
{
    double origin_x;
    double origin_y;
    double origin_z;
    double rotation_x;
    double rotation_y;
    double rotation_z;
};
double theta_L_wheel = 0;
double theta_R_wheel = 0;
double theta_car = 0;
double x_car = 0;
double y_car = 0;

/* --- 初始化關節位置與角度 --- */
void joint_init(void);
/* --- 廣播關節tf資訊 --- */
void set_joint_tf(pose, const char*, const char*);
/* --- 設定關節位置與角度函數 --- */
pose set_pose(double,double,double,double,double,double);
/* --- 計算中心速度加速度 --- */
double calc_cneter_acceleration(void);
/* --- 計算兩輪新的旋轉角度 --- */
double calc_wheel_angular(double);

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_simulation3");
    ros::NodeHandle n;
    tf::TransformBroadcaster broadcaster;
    ros::Rate r(sleep_hz);
    double vl=0,vr=0;
    while(ros::ok())
    {
        joint_init();
        vl=Center_Velocity-((Angular_Velocity*wheel_distance)/2);
        vr=Center_Velocity+((Angular_Velocity*wheel_distance)/2);
        theta_L_wheel = theta_L_wheel + calc_wheel_angular(vl);
        theta_R_wheel = theta_R_wheel + calc_wheel_angular(vr);
        x_car=x_car+( ((vl+vr)/2)*cos(theta_to_rad(theta_car)) )/sleep_hz ;
        y_car=y_car+( ((vl+vr)/2)*sin(theta_to_rad(theta_car)) )/sleep_hz ;
        theta_car=theta_car+( rad_to_theta(Angular_Velocity)/sleep_hz );
        r.sleep();
    }
}

void joint_init(void)
{
    set_joint_tf(set_pose(0, 0, 0, 0, 0, 0), "map", "odom");
    set_joint_tf(set_pose(x_car, y_car, 0.075, 0, 0, theta_car), "odom", "base_link");
    set_joint_tf(set_pose(0.125, 0, -0.05, 0, 0, 0), "base_link", "ball_link");
    set_joint_tf(set_pose(0.13, 0, 0.07, 0, 0, 0), "base_link", "laser_link");
    set_joint_tf(set_pose(-0.1, 0.125, -0.025, 0, -90, theta_L_wheel), "base_link", "wheel_R");
    set_joint_tf(set_pose(-0.1, -0.125, -0.025, 0, -90, theta_R_wheel), "base_link", "wheel_L");
}

void set_joint_tf(pose p,const char *parent,const char *child)
{
    tf::TransformBroadcaster broadcaster;
    tf::Transform tf;
    tf::Vector3 v3;
    v3.setValue( p.origin_x, p.origin_y, p.origin_z );
    tf::Quaternion q;
    q.setEuler( theta_to_rad(p.rotation_x), theta_to_rad(p.rotation_y), theta_to_rad(p.rotation_z) );
    tf.setOrigin(v3);
    tf.setRotation(q);  
    broadcaster.sendTransform(
        tf::StampedTransform(
            tf,
            ros::Time::now(),
            parent, 
            child));
}

pose set_pose(double ox,double oy,double oz,double rx,double ry,double rz)
{
    pose p;
    p.origin_x = ox;
    p.origin_y = oy;
    p.origin_z = oz;
    p.rotation_x = rx;
    p.rotation_y = ry;
    p.rotation_z = rz;
    return p;
}

double calc_cneter_acceleration(void)
{
    double acc=0;
    acc = ( 360.0*Center_Velocity/wheel_circumference )/sleep_hz;
    return acc;
}

double calc_wheel_angular(double v)
{
    double acc_angular=0;
    acc_angular = ( 360.0*v/wheel_circumference )/sleep_hz;
    return acc_angular;
}