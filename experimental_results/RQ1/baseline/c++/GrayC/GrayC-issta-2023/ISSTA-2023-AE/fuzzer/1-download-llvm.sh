#!/bin/bash
working_folder=$1
llvm_ver=$2

# Downloading LLVM and Csmith sources, building Csmith
# TMP_SOURCE_FOLDER=$(mktemp -d $working_folder/.sources_$2VL.XXXXXXX.tmp)
# cd $TMP_SOURCE_FOLDER
# ## LLVM
# if [ -z "$2" ] ; then
#         echo " >> Checking out main branch"
#         git clone --depth 1 -b main https://github.com/llvm/llvm-project.git
# else
#         echo " >> Checking out releases/gcc-$llvm_ver tag"
#         git clone --depth 1 -b release/$llvm_ver.x https://github.com/llvm/llvm-project.git
# fi
TMP_SOURCE_FOLDER=$(mktemp -d $working_folder/.sources_$2VL.XXXXXXX.tmp)
cd $TMP_SOURCE_FOLDER
cp ../../llvm-project-release-12.x.zip .
unzip -q llvm-project-release-12.x.zip
mv llvm-project-release-12.x llvm-project

## fix paths
mv $TMP_SOURCE_FOLDER/llvm-project/llvm $TMP_SOURCE_FOLDER/llvm-source
mv $TMP_SOURCE_FOLDER/llvm-project/clang $TMP_SOURCE_FOLDER/llvm-source/tools/clang
rm -rf $TMP_SOURCE_FOLDER/llvm-project

## CSMITH:
cd $TMP_SOURCE_FOLDER
# git clone https://github.com/csmith-project/csmith.git
cp /home/ubuntu/baseline/csmith/csmith-csmith-2.3.0.tar.gz .
tar -xf csmith-csmith-2.3.0.tar.gz
mv csmith-csmith-2.3.0 csmith
cd ./csmith

echo " >> Downloaded LLVM $llvm_ver and Csmith into $TMP_SOURCE_FOLDER."
