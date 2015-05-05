#! /bin/bash

CONTAINER=testfire

#bind the usb for the leap
CONTAINER_ID=$(sudo docker run -ti  \
				-e DISPLAY=$DISPLAY \
				-v /temp/.X11-unix:/temp/.X11-unix \
				$CONTAINER)

echo $(sudo docker logs $CONTAINER_ID | sed -n 1p)

echo $(sudo docker port $CONTAINER_ID 22)

#--device=/dev/ttyUSB0:/dev/ttyUSB0