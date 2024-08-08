
#!/bin/bash
# Please alternate RASPBERRY_PI_HOST value when the ip adress of Raspberry Pi is changed
# Variables
# 도커 빌드하고 도커 안에서 실행파일 qt를 빌드하고 이거를 호스트로 복사하고 호스트에서 타겟으로
DOCKER_IMAGE_NAME="qt-app-builder"
DOCKER_CONTAINER_NAME="qt-app-build-container"
BUILD_DIR="/project/build"
HOST_COPY_DIR="."
RASPBERRY_PI_USER="falcon"
RASPBERRY_PI_HOST="192.168.86.24"
RASPBERRY_PI_TARGET_DIR="/home/${RASPBERRY_PI_USER}"
QT_APP_NAME="InstrumentCluster"
echo "Building the Docker container..."
docker build -t $DOCKER_IMAGE_NAME .
echo "Running the Docker container..."
docker run --name $DOCKER_CONTAINER_NAME $DOCKER_IMAGE_NAME
echo "Copying the built application to the host machine..."
docker cp $DOCKER_CONTAINER_NAME:$BUILD_DIR/$QT_APP_NAME $HOST_COPY_DIR
echo "Removing the Docker container..."
docker rm $DOCKER_CONTAINER_NAME
echo "Transferring the built application to the Raspberry Pi..."
scp -r $HOST_COPY_DIR/$QT_APP_NAME $RASPBERRY_PI_USER@$RASPBERRY_PI_HOST:$RASPBERRY_PI_TARGET_DIR
echo "Removing executable file from host..."
rm $HOST_COPY_DIR/$QT_APP_NAME
echo "Done"
