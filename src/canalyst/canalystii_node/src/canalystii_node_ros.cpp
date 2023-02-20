#include "ros/ros.h"
#include "canalystii_node.h"

int main(int argc, char **argv){
    ros::init(argc, argv, "canalystii_node");
    CANalystii_node can_node;
    ros::Rate loop_rate(100);
    
    if(!can_node.start_device()){
        ROS_WARN("device starts error");
        return -1;
    }

    VCI_INIT_CONFIG vci_conf;
    vci_conf.AccCode = 0x80000008;
    vci_conf.AccMask = 0xFFFFFFFF;
    vci_conf.Filter = 1;//receive all frames
    vci_conf.Timing0 = 0x00;
    vci_conf.Timing1 = 0x1C;//baudrate 500kbps
    vci_conf.Mode = 0;//normal mode
    unsigned int can_idx = 0;
    if(!can_node.init_can_interface(can_idx,vci_conf)){
        ROS_WARN("device port(%d) init error",can_idx);
        return -1;
    }
    else ROS_INFO("device port(%d) init Success",can_idx);

    can_idx = 1;
    if(!can_node.init_can_interface(can_idx,vci_conf)){
        ROS_WARN("device port(%d) init error",can_idx);
        return -1;
    }
    else ROS_INFO("device port(%d) init Success",can_idx);

    // ROS_INFO("listening to can bus");
    VCI_CAN_OBJ can_obj_Rx;
    VCI_CAN_OBJ can_obj_Tx;
    can_obj_Tx.ID=0;
    can_obj_Tx.SendType=1;
    can_obj_Tx.RemoteFlag=0;
    can_obj_Tx.ExternFlag=0;
    can_obj_Tx.DataLen=8;
	for(int i=0; i<can_obj_Tx.DataLen; i++) can_obj_Tx.Data[i]=(i+1);

    while(ros::ok())
    {
        unsigned int recv_len = 1;
        if(can_node.send_can_frame(0,can_obj_Tx,recv_len))
        {   
            // ROS_INFO("device port0 send Success");
            //int len = can_node.receive_can_frame(can_idx,can_obj,recv_len,0);
            if(can_node.receive_can_frame(1,can_obj_Rx,recv_len,0)){
                //ROS_INFO("received:%u",can_obj.ID);
                canalystii_node_msg::can msg = CANalystii_node::can_obj2msg(can_obj_Rx);
                can_node.can_msg_pub_.publish(msg);
            }
        }
        can_obj_Tx.ID++;
        ros::spinOnce();
		loop_rate.sleep();
    }
    //ros::spin();
    return 0;
}