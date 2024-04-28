#!/bin/bash

function info(){
    echo -e "[INFO] \e[96m$1\e[0m"
}

function warn(){
    echo -e "[WARN] \e[93m$1\e[0m"
}

function err(){
    echo -e "[ERR] \e[91m$1\e[0m"
}
