FROM ubuntu:trusty
MAINTAINER yetanotherminion <yetanotherminion@gmail.com>

RUN apt-get update

RUN export uid=1000 gid=1000 && \
    mkdir -p /home/leap && \
    echo "leap:x:${uid}:${gid}:Leap,,,:/home/leap:/bin/bash" >> /etc/passwd && \
    echo "leap:x:${uid}:" >> /etc/group && \
    echo "leap ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/leap && \
    chmod 0440 /etc/sudoers.d/leap && \
    chown ${uid}:${gid} -R /home/leap

ADD Leap_Motion_SDK_Linux_2.2.5.tgz /home/leap

RUN cd /home/leap/LeapDeveloperKit*  \
	dpkg --install Leap-*-x64.deb

RUN apt-get update


USER developer
ENV HOME /home/developer
ENTRYPOINT ["/bin/bash"]
