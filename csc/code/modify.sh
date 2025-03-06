#!/bin/bash

file="$1"
src_s="$2"
dst_s="$3"

sed -i "s/${2}/${3}/g" "${file}"