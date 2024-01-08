#!/usr/bin/env bash
arduino-cli compile --upload . && arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200
