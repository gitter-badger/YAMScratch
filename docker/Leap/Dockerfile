FROM ubuntu:trusty
MAINTAINER yetanotherminion <yetanotherminion@gmail.com>

RUN apt-get update

ADD Leap_Motion_SDK_Linux_2.2.0.tgz /leap

RUN cd /leap/LeapDeveloperKit*  \
	dpkg --install Leap-*-x64.deb

RUN apt-get install -y firefox

CMD /usr/bin/firefox

#ENTRYPOINT ["/bin/bash"]
