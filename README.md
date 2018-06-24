## Autonomous Detection and Tracking of a High-Speed Ground Vehicle using a Quadrotor UAV

This project presents 

* A quadrotor UAV system capable of detecting and tracking a high speed moving ground vehicle.
* Autonomous landing on a ground vehicle moving at high speeds with low or moderate accelerations. 

We assumed that no a prior information about the movement of the high-speed ground vehicle is available and that the quadrotor UAV is equipped with minimal sensor suite of a stereo camera and an IMU. We relied on state-of-the-art deep learning based object detection, stereo image processing algorithm, and kernelized correlation filtering algorithms to detect and track a moving vehicle from raw image data. We also developed an adaptive position control strategy that estimates the vehicle motion and provides a feed-forward compensation. Comparison of the developed adaptive controller with a standard PID controller revealed that adaptive controller has better tracking performance.
