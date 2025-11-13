#!/usr/bin/env python3
import sys, ctypes, json, os, pathlib

def check_symbols(libpath):
    required = ["vkCreateInstance","vkGetInstanceProcAddr","vkEnumeratePhysicalDevices","vkCreateDevice"]
    lib = ctypes.CDLL(libpath)
    for s in required:
        if not hasattr(lib, s):
            raise SystemExit(f"Missing symbol {s} in {libpath}")
    print("Symbol check passed")

def check_manifest(manifest):
    data = json.loads(open(manifest).read())
    if "ICD" not in data or "library_path" not in data["ICD"]:
        raise SystemExit("Invalid manifest")
    print("Manifest check passed")

def check_shaders(pkgdir):
    sp = pathlib.Path(pkgdir) / "assets" / "shaders"
    if not sp.exists():
        print("No shaders shipped (optional)")
        return
    for f in sp.iterdir():
        print("Found shader:", f.name)

if __name__ == "__main__":
    lib = sys.argv[1]
    manifest = sys.argv[2]
    pkg = os.path.dirname(lib).rsplit('/',2)[0] if '/' in lib else '.'
    if not os.path.exists(lib):
        raise SystemExit("lib not found: " + lib)
    check_symbols(lib)
    check_manifest(manifest)
    check_shaders(pkg)
    print("Validation OK")
