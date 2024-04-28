#!/bin/bash

# set print style
export SCRIPTDIR=$(pwd)
source $SCRIPTDIR/_message.sh

if [ $EUID -ne 0 ]; then
    echo "Please run as root or sudo"
    exit -1
fi

info "Delete result/ files..."
sudo rm -f result/*
info "Delete log/ files..."
sudo rm -f log/*
info "Delete tmp model files..."
sudo rm -r -f ./*.model
