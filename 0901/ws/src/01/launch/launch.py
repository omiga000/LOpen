#!/usr/bin/env python3
# hybrid_launch.py

from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 发布者节点
        Node(
            package='01',  # 你的包名
            executable='Pub_node',   # 可执行文件
            name='pub_node',          # 节点名称（可选）
            output='screen',          # 输出到终端
            parameters=[{            # 可以传递参数
                'message': 'Hello from launch',
                'count': 42,
                'image_path': '/home/epsilon/LOpencv/0901/ws/Draw.jpg'
            }]
        ),
        
        # 订阅者节点
        Node(
            package='01',
            executable='Sub_node',
            name='sub_node',
            output='screen'
        ),
    ])