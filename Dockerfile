FROM ubuntu:noble

RUN apt update -y
RUN apt upgrade -y

# General build requirments
RUN apt install -y lsb-release wget meson ninja-build gcovr git cmake \
                   software-properties-common gnupg build-essential bear

# NEW CPP 23 REQUIREMENT
RUN apt install -y cpp-14 libstdc++-14-dev libc++-dev libc++abi-dev

# LLVM
RUN apt install -y libcapture-tiny-perl libdatetime-perl
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 20 all

# AdaptiveCpp
RUN apt install -y libboost-all-dev
RUN git clone https://github.com/AdaptiveCpp/AdaptiveCpp.git -b v25.02.0
RUN CXX=clang++-20 cmake -B AdaptiveCpp/build -S AdaptiveCpp -DCMAKE_INSTALL_PREFIX=/opt/AdaptiveCpp
RUN (cd AdaptiveCpp/build; CXX=clang++-20 make install -j 4)
ENV LD_LIBRARY_PATH=/opt/AdaptiveCpp/lib/
ENV CPLUS_INCLUDE_PATH=/opt/AdaptiveCpp/include/AdaptiveCpp/
ENV PATH=$PATH:/opt/AdaptiveCpp/bin

