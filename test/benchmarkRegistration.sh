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

function register() {
    local fixedImg="$1"
    local movingImg="$2"

    local fixedImgAbsPath="$(realpath "$fixedImg")"
    local fixedImgBn="$(basename "$fixedImgAbsPath")"
    local fixedImgBnWithoutExtension="$(echo "$fixedImgBn" | cut -d '.' -f 1)"

    local movingImgAbsPath="$(realpath "$img")"
    local movingImgBn="$(basename "$movingImgAbsPath")"
    local movingImgBnWithoutExtension="$(echo "$movingImgBn" | cut -d '.' -f 1)"

    local outputPrefix="${movingImgBnWithoutExtension}_${fixedImgBnWithoutExtension}"
    local outputImg="${OUTPUT_DIRECTORY}/${outputPrefix}.tif"
    local outputLog="${OUTPUT_DIRECTORY}/${outputPrefix}.log"

    # Measure and execute registration
    echo "Register ${movingImgBn} to ${fixedImgBn}..."
    start=$(date +%s.%N)
    $REGISTRATION -g "$GRADIENT_LEARNING_RATE" -i "$GRADIENT_MAXITER" -t "$GRADIENT_TOLERANCE" -s "$GRADIENT_STEP" "$fixedImgAbsPath" "$movingImgAbsPath" "$outputImg" > "$outputLog" 2> /dev/null
    end=$(date +%s.%N)
    duration=$(echo "$end - $start" | bc)

    # Extract and build CSV benchmark
    echo "${fixedImgAbsPath},${movingImgAbsPath},${GRADIENT_LEARNING_RATE},${duration},$(extractLogInfos "$outputLog")" >> "$OUTPUT_BENCHMARK"
}

# =============================================
#                   ARGUMENTS
# =============================================
OUTPUT_DIRECTORY="$1"
IMAGES=("${@:2}")

if [ ! -d "$OUTPUT_DIRECTORY" ]; then
    echo "Output directory '$OUTPUT_DIRECTORY' is not a directory" 1>&2
    exit 1
fi

for img in "${IMAGES[@]}"; do
    if [ ! -f "$img" ]; then
        echo "Image '$img' is not a file" 1>&2
        exit 1
    fi
done

OUTPUT_BENCHMARK="${OUTPUT_DIRECTORY}/benchmark.csv"

# =============================================
#                   MAIN LOOP
# =============================================
echo "fixed_img,moving_img,gradient_step,time,iteration_i,tx_i,ty_i,tz_i,rx_i,ry_i,rz_i,metric_i,iteration_f,tx_f,ty_f,tz_f,rx_f,ry_f,rz_f,metric_f" > "$OUTPUT_BENCHMARK"
job=0
for fixedImg in "${IMAGES[@]}"; do
    for img in "${IMAGES[@]}"; do
        register "$fixedImg" "$img" &

        job=$((job + 1))
        if [ "$job" -ge "$JOBS" ]; then
            wait
            job=0
        fi
    done
done
wait
