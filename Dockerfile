FROM centos:7.5.1804

# Run out-of-source builds
WORKDIR /apps/src/bld

# Install standard dependencies
RUN yum install -y epel-release
RUN yum install -y zeromq-devel curl cmake gcc gcc-c++ make libevent-devel libuuid-devel ruby-devel rpm-build

ENV VERSION 1.2.3
ENV DISTRIB_PACKAGE_QUALIFIER el7
ENV PACKAGE_TYPE rpm

# Install OpenMAMA
RUN curl -1sLf 'https://dl.cloudsmith.io/public/openmama/openmama/cfg/setup/bash.rpm.sh' | bash
# RUN curl -1sLf 'https://dl.cloudsmith.io/public/openmama/openmama/cfg/setup/bash.deb.sh' | bash
RUN yum install -y openmama
RUN gem install -N fpm

# Add the rest of the source code here so code changes don't trigger dependency regeneration
ADD . /apps/src

# Perform the build and install
RUN cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_INSTALL_PREFIX=/apps/install/opt/openmama .. && make install

RUN fpm -s dir \
        -t $PACKAGE_TYPE \
        -m "contact@cascadium.io" \
        -C /apps/install \
        --name openmama \
        --version $VERSION \
        --iteration 1 \
        -d openmama -d zeromq -d libevent -d libuuid \
        -p openmama-zmq-$VERSION-1.$DISTRIB_PACKAGE_QUALIFIER.x86_64.$PACKAGE_TYPE \
        --description "OpenMAMA middleware bridge for zeromq" .

# Copy the artifact to the release directory (probably mounted)
CMD mkdir -p /apps/release && cp /opt/openmama/lib/libmamazmqimpl.so /apps/release
