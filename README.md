# 👻 GhostStunt

A fork of [ToastStunt](https://github.com/lisdude/toaststunt) featuring various enhancements and ill-advised schemes.

### This is experimental software. Caveat emptor!

## Build Instructions
### **Docker**
```bash

# Build from git repository
docker build -t ghoststunt https://github.com/StraylightRunMOO/GhostStunt.git

# Or build locally
git clone https://github.com/StraylightRunMOO/GhostStunt.git
cd GhostStunt && docker build -t ghoststunt .

# Run with default options
docker run --rm -dt -p 7777:7777/tcp localhost/ghoststunt:latest

# Run with additional options passed to the 'moo' command 
docker run --rm -dt --name ghoststunt-server -p 7777:7777/tcp \
  localhost/ghoststunt:latest
  -i /data/files \
  -x /data/exec \
  /data/db/ghostcore.db /data/db/ghostcore.out.db 7777

# Mount folder as a volume, map to port 8888 
mkdir -p data/db
docker run --rm -dt -v=$(pwd)/data/:/data/ -p 8888:7777/tcp localhost/ghoststunt:latest

```

Additional build instructions and information can be found in the original [ToastStunt documentation](https://github.com/StraylightRunMOO/GhostStunt/docs/ToastStunt.md).
