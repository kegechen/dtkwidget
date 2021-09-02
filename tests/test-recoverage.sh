#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report

#cd ../
#rm -rf $BUILD_DIR
#mkdir $BUILD_DIR
#cd $BUILD_DIR
#qmake .. CONFIG+=debug
#make -j$(nproc)
#cd ../tests/
#rm -rf $BUILD_DIR

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../../ CONFIG+=debug
export ASAN_OPTIONS=halt_on_error=0
make check -j$(nproc)

lcov -d ./ -c -o coverage_all.info
lcov --extract coverage_all.info $EXTRACT_ARGS --output-file coverage.info
filter_files=(
# deprecated
"*/src/widgets/dimagebutton*"
"*/src/widgets/dbaseexpand*"
"*/src/widgets/dexpandgroup*"
"*/src/widgets/dsegmentedcontrol*"
"*/src/widgets/dshortcutedit*"
"*/src/widgets/dtoast*"
"*/src/widgets/dsimplelistitem*"
"*/src/widgets/dswitchlineexpand*"
"*/src/widgets/dapplicationhelper*"
"*/src/util/dregionmonitor*"
"*/src/util/dtrashmanager*"
"*/src/util/dthumbnailprovider*"
# depends xcb, etc...
"*/dthememanager*"
"*/dsimplelistview*"
"*/dmainwindow*"
"*/dapplication*"
"*/xutil*"
"*/private/startupnotifications*"
"*/private/keyboardmonitor*"
)
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build/src*" ${filter_files[*]} --output-file coverage.info
genhtml -o ../../tests/$REPORT_DIR coverage.info

cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info

test -e ./build/tests/asan.log* && mv ./build/tests/asan.log* ./build/asan_dtkwidget.log || echo "Not detected any memory leak."
