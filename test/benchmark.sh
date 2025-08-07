#!/bin/bash

# =============================================
#               GLOBAL VARIABLES
# =============================================
REGISTRATION="${REGISTRATION:-"tifo-project"}"
GRADIENT_LEARNING_RATE="${GRADIENT_LEARNING_RATE:-"1e-2"}"
GRADIENT_STEP="${GRADIENT_STEP:-"1e-2"}"
GRADIENT_TOLERANCE="${GRADIENT_TOLERANCE:-"1e-2"}"
GRADIENT_MAXITER="${GRADIENT_MAXITER:-"1000"}"
JOBS="${JOBS:-"$(nproc --all)"}"

if [ ! -x "$REGISTRATION" ]; then
    echo "Registration process '$REGISTRATION' is not executable" 1>&2
    exit 1
fi


# =============================================
#                   FUNCTIONS
# =============================================
function extractVectorParameters() {
    local vector="$1"
    local coordinates="$(echo "$vector" | sed 's/Vector3f//g' | sed 's/(//g' | sed 's/)//g')"
    local vx="$(echo "$coordinates" | cut -d ';' -f 1)"
    local vy="$(echo "$coordinates" | cut -d ';' -f 2)"
    local vz="$(echo "$coordinates" | cut -d ';' -f 3)"
    echo "${vx},${vy},${vz}"
}

function extractLogLineInfos() {
    local line="$1"

    # Extract iteration
    local iteration="$(echo "$line" | cut -d ',' -f 1)"

    # Extract translation & rotation
    local transformData="$(echo "$line" | cut -d ',' -f 2)"
    local translationVector="$(echo "$transformData" | cut -d ' ' -f 1 | cut -d '=' -f 2)"
    local translationParameters="$(extractVectorParameters "$translationVector")"
    local rotationVector="$(echo "$transformData" | cut -d ' ' -f 2 | cut -d '=' -f 2)"
    local rotationParameters="$(extractVectorParameters "$rotationVector")"

    # Extract metric value
    local metric="$(echo "$line" | cut -d ',' -f 3)"

    echo "${iteration},${translationParameters},${rotationParameters},${metric}"
}

function extractLogInfos() {
    local logFile="$1"

    local initialState="$(extractLogLineInfos "$(head "$logFile" -n 2 | tail -n 1)")"
    local finalState="$(extractLogLineInfos "$(tail "$logFile" -n 1)")"

    echo "${initialState},${finalState}"
}

# =============================================
#                   ARGUMENTS
# =============================================
OUTPUT_DIRECTORY="$1"
if [ ! -d "$OUTPUT_DIRECTORY" ]; then
    echo "Output directory '$OUTPUT_DIRECTORY' is not a directory" 1>&2
    exit 1
fi

OUTPUT_BENCHMARK="${OUTPUT_DIRECTORY}/benchmark.csv"


# =============================================
#                   MAIN LOOP
# =============================================
echo "fixed_img,moving_img,learning_rate,time,iteration_i,tx_i,ty_i,tz_i,rx_i,ry_i,rz_i,metric_i,iteration_f,tx_f,ty_f,tz_f,rx_f,ry_f,rz_f,metric_f" > "$OUTPUT_BENCHMARK"
for i in $(seq 1 1 7); do
    for j in $(seq 1 1 7); do
        fixedImg="../../../tifo-project-samples/${i}-*.fr.tif"
        movingImg="../../../tifo-project-samples/${j}-*.fr.tif"
        # shellcheck disable=SC2086
        fixedImgBn="$(basename $fixedImg)"
        # shellcheck disable=SC2086
        movingImgBn="$(basename $movingImg)"
        outputImg="f${i}-m${j}.tif"
        outputLog="f${i}-m${j}.log"
        errorLog="f${i}-m${j}.stderr.log"

        # shellcheck disable=SC2086
        echo "Register" $movingImgBn "to" $fixedImgBn

        start=$(date +%s.%N)
        # shellcheck disable=SC2086
        $REGISTRATION -g "$GRADIENT_LEARNING_RATE" -i "$GRADIENT_MAXITER" -t "$GRADIENT_TOLERANCE" -s "$GRADIENT_STEP" $fixedImg $movingImg "${OUTPUT_DIRECTORY}/${outputImg}" > "${OUTPUT_DIRECTORY}/$outputLog" 2> "${OUTPUT_DIRECTORY}/$errorLog"
        end=$(date +%s.%N)
        duration=$(echo "$end - $start" | bc)

        echo "$fixedImgBn","$movingImgBn","${GRADIENT_LEARNING_RATE}","${duration}","$(extractLogInfos "${OUTPUT_DIRECTORY}/$outputLog")" >> "$OUTPUT_BENCHMARK"
    done
done