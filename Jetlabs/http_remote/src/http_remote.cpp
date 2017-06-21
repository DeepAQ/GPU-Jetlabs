#include <string.h>
#include <microhttpd.h>
#include <ros/ros.h>
#include <thread>
#include <chrono>
#include "geometry_msgs/Twist.h"
#include "std_msgs/Int16.h"

#define LINEAR_SPEED 10
#define ANGULAR_SPEED 1
#define OBJECT_DIST 70

#define HTTP_PORT 8888

using namespace std;

int state = 0;
geometry_msgs::Twist vel_msg;
ros::Publisher vel_pub;

void send_vel(float linear, float angular)
{
	vel_msg.linear.x = linear;
	vel_msg.angular.z = angular;
	vel_pub.publish(vel_msg);
	ROS_INFO("Sent vel %f, %f", linear, angular);
}

int answer_to_connection(void *cls, struct MHD_Connection *connection,
                         const char *url,
                         const char *method, const char *version,
                         const char *upload_data,
                         size_t *upload_data_size, void **con_cls)
{
	printf("%s %s %s\n", version, method, url);

	if (strcmp(url, "/forward") == 0) {
		state = 1;
	} else if (strcmp(url, "/reverse") == 0) {
		state = 2;
	} else if (strcmp(url, "/turn") == 0) {
		state = 3;
	} else if (strcmp(url, "/stop") == 0) {
		state = 0;
	}

	const char *page = "<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"></head><body><div style=\"font-size:48px;\"><a href=\"/forward\">Forward</a><br><a href=\"/reverse\">Reverse</a><br><a href=\"/turn\">Turn</a><br><a href=\"/stop\">Stop</a></div></body></html>";
	struct MHD_Response *response;
	int ret;
	response = MHD_create_response_from_buffer(strlen(page), (void*) page, MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);
	return ret;
}

void looper()
{
	ROS_INFO("Looper started");
	while (1) {
		switch (state) {
		case 1:
			ROS_INFO("Forward");
			send_vel(LINEAR_SPEED, 0);
			break;
		case 2:
			ROS_INFO("Reverse");
			send_vel(-LINEAR_SPEED, 0);
			break;
		case 3:
			ROS_INFO("Turn");
			send_vel(0, ANGULAR_SPEED);
			break;
		default:
			ROS_INFO("Stop");
			send_vel(0, 0);
		}
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

int main(int argc, char **argv)
{
  struct MHD_Daemon *daemon;
  daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, HTTP_PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon) return 1;
  ROS_INFO("HTTP Daemon started");
  ROS_INFO("ROS Init");
  ros::init(argc, argv, "sense_and_avoid");

  ros::NodeHandle nh;
  vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  thread lthread(looper);

  ROS_INFO("ROS Spin");
  ros::spin();
}
