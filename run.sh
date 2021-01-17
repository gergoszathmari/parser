#!/bin/bash

if [[ -z "$WT_TMP_DIR" ]]; then
    export WT_TMP_DIR='config/tmp'
fi

./parser --docroot config --http-listen 0.0.0.0:14080 --resources config/resources/wt --approot config/approot