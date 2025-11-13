#!/usr/bin/env python3
import shutil, sys, pathlib, subprocess

def package(pkgdir, out):
    p = pathlib.Path(pkgdir)
    if not p.exists():
        raise SystemExit("pkg dir missing")
    subprocess.run(["tar","--zstd","-cvf",out,"-C",pkgdir,"."], check=True)
    print("Packaged", out)

if __name__ == "__main__":
    package(sys.argv[1], sys.argv[2])
