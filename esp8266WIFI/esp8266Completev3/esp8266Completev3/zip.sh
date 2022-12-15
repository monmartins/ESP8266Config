rm -rf ./data
mkdir ./data
cp -rp ./backup/*  ./data
gzip -r ./data/*