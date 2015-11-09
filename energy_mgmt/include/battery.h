#ifndef BATTERY_H
#define BATTERY_H

#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <diagnostic_msgs/DiagnosticArray.h>
#include <energy_mgmt/battery_state.h>
#include <explorer/Speed.h>

class battery
{
public:
    /**
     * Constructor.
     */
    battery();

    /**
     * Compute the battery state.
     */
    void compute();

    /**
     * Outputs the battery soc to the console.
     */
    void output();

    /**
     * Publishes a message containing the battery state.
     */
    void publish();

    /**
     * The battery state.
     * This includes:
     *  - bool charging
     *  - float32 soc
     *  - float32 remaining_time_charge
     *  - float32 remaining_time_run
     *  - float32 remaining_distance
     */
    energy_mgmt::battery_state state;


private:
    /**
     * The node handle.
     */
    ros::NodeHandle nh;

    /**
     * Subscribers for the required topics.
     */
    ros::Subscriber sub_charge, sub_cmd_vel, sub_speed;

    /**
     * Publishers.
     */
    ros::Publisher pub_battery;

    /**
     * Battery charge in Wh.
     * Charge is the current battery charge, charge_max is the charge if the battery is fully charged.
     */
    double charge, charge_max;

    /**
     * The power consumption of the robot.
     * Power_charging is the power that the robot is charged with.
     * Power_moving is the power consumption of the robot while in motion.
     * Power_standing is the power consumption of the robot while it is standing still.
     */
    double power_charging, power_moving, power_standing;

    /**
     * Speed of the robot.
     * The values are received from subscribed topics.
     */
    double speed_linear, speed_angular, speed_avg;

    /**
     * Total time the robot has been standing and moving.
     * This excludes the times while the robot is recharging.
     */
    double time_moving, time_standing;

    /**
     * Percentage of time the robot has been standing and moving.
     */
    double perc_moving, perc_standing;

    /**
     * Whether or not the soc output has been shown.
     * This is necessary so that each value is shown only once.
     */
    bool output_shown;

    /**
     * The last time that the computations where carried out.
     */
    ros::Time time_last;

    /**
     * Callback that is triggered when the robot should start recharging.
     */
    void charge_callback(const std_msgs::Empty::ConstPtr &msg);

    /**
     * Callback that is used to determine if robot is standing or moving.
     * In the cmd_vel message there are only two important parameters, the x-linear value and the z-angular value.
     * When they are zero the robots stands still. If the x-value is unequal to zero then the robot moves forward or
     * backward and if the z-value is unequal to zero the robot rotates.
     * When the robot stands still it consumes less energy and when it moves it consumes more energy.
     */
    void cmd_vel_callback(const geometry_msgs::Twist &msg);

    /**
     * Callback that is used to get the average speed of the robot.
     */
    void speed_callback(const explorer::Speed &msg);
};

#endif  /* BATTERY_H */
