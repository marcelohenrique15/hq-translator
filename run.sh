if [ -d "build" ]; then
  echo "\nBuild já foi criada.\n"

else
  echo "\nCriando pasta Build...\n"
  mkdir build
fi

echo "\nCompilando...\n"

cd build
cmake ..
make
./main
cd ..