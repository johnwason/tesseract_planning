#! /bin/bash
apt update -qq
apt install -y --no-install-recommends software-properties-common
add-apt-repository -y ppa:ros-industrial/ppa
apt update -qq
