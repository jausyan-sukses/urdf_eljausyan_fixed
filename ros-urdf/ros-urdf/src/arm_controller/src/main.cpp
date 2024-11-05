#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <vector>
#include <unistd.h> 

int main(int argc, char** argv) {
    ros::init(argc, argv, "joint_state_publisher");
    ros::NodeHandle nh;
    ros::Publisher joint_state_pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);
    ros::Rate loop_rate(5); 

    int step = 0;
    bool is_letter_done = false; 
    std::vector<std::vector<double>> elja_positions = {
        // "E"
        {0.0, 1.0, 0.0}, {0.0, 1.0, -1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.5},
        // "L"
        {0.5, 0.0, 0.0}, {0.5, 0.0, -1.0},
        // "J"
        {0.0, -1.0, 0.5}, {0.0, -1.0, -1.0},
        // "A"
        {0.0, 1.2, 0.0}, {0.5, 1.0, -0.5}, {0.5, 1.0, 0.5}
    }; 

    std::vector<int> letter_end_steps = {3, 5, 7, 10};

    while (ros::ok()) {
        sensor_msgs::JointState joint_state;
        joint_state.header.stamp = ros::Time::now();
        joint_state.name = {"hip", "shoulder", "elbow"};
        joint_state.position.resize(3);

        joint_state.position[0] = elja_positions[step][0]; // hip 
        joint_state.position[1] = elja_positions[step][1]; // shoulder 
        joint_state.position[2] = elja_positions[step][2]; // elbow 

        joint_state_pub.publish(joint_state);
        printf("Step %d: hip=%.2f, shoulder=%.2f, elbow=%.2f\n", step, elja_positions[step][0], elja_positions[step][1], elja_positions[step][2]);

        if (std::find(letter_end_steps.begin(), letter_end_steps.end(), step) != letter_end_steps.end()) {
            is_letter_done = true; 
        }

        if (is_letter_done) {
            ros::Duration(3.0).sleep(); // Jeda 5 detik
            is_letter_done = false; 
        }

        step++;
        if (step >= elja_positions.size()) {
            step = 0; // Restart movement 
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
