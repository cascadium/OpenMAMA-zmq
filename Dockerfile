FROM centos:7.5.1804

# Run out-of-source builds
WORKDIR /apps/src/bld

# Install standard dependencies
RUN yum install -y epel-release
RUN yum install -y zeromq-devel curl cmake gcc gcc-c++ make libevent-devel libuuid-devel

# Install OpenMAMA
RUN curl -1sLf 'https://dl.cloudsmith.io/public/openmama/openmama/cfg/setup/bash.rpm.sh' | bash
RUN yum install -y openmama

# Add the rest of the source code here so code changes don't trigger dependency regeneration
ADD . /apps/src

# Perform the build and install
RUN cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo .. && make install

# Copy the artifact to the release directory (probably mounted)
CMD mkdir -p /apps/release && cp /opt/openmama/lib/libmamazmqimpl.so /apps/release
