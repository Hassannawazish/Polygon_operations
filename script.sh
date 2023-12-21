BUILD_FOLDER="build"
if [ ! -d "$BUILD_FOLDER" ]; then
    mkdir "$BUILD_FOLDER"
    echo "Build folder created."
else
    echo "Build folder already exists."
fi

cd $BUILD_FOLDER
cmake ..
make -j12
./PolygonUnionExample 