#! /bin/bash

CONTAINER=yam/leapv1

#bind the usb for the leap
CONTAINER_ID=$(sudo docker run -t -i  \
				$CONTAINER)

echo $(sudo docker logs $CONTAINER_ID | sed -n 1p)

echo $(sudo docker port $CONTAINER_ID 22)

#--device=/dev/ttyUSB0:/dev/ttyUSB0