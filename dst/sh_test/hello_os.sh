#!/bin/bash

src_file="$1"
dst_file="$2"

sed -n '8p' ${src_file} > ${dst_file}
sed -n '32p' ${src_file} >> ${dst_file}
sed -n '128p' ${src_file} >> ${dst_file}
sed -n '512p' ${src_file} >> ${dst_file}
sed -n '1024p' ${src_file} >> ${dst_file}


