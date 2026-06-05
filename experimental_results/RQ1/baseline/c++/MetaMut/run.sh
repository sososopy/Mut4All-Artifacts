rm -rf workspace
mkdir -p workspace

python3.10 metamut/fuzzer/run.py -j 4 \
  --duration 86400 \
  --seeds-dir $(pwd)/seed \
  --cc-opt=-O2 \
  --wdir workspace

rm -rf results

python3.10 extract_res.py workspace 3600