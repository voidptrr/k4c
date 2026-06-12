{
  lib,
  stdenv,
  cmake,
  ninja,
}:
stdenv.mkDerivation {
  pname = "ckit";
  version = "0.1.0";

  src = ../..;

  nativeBuildInputs = [
    cmake
    ninja
  ];

  doCheck = true;

  meta = {
    description = "C utility library";
    license = lib.licenses.mit;
    platforms = lib.platforms.linux;
  };
}
