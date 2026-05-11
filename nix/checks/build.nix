{pkgs}:
pkgs.runCommand "build-all-targets-check" {
  src = ../../.;
  nativeBuildInputs = [
    pkgs.cmake
    pkgs.gcc
    pkgs.coreutils
    pkgs.findutils
    pkgs.gnused
  ];
} ''
  cp -r "$src" source
  chmod -R +w source
  cd source

  targets="$(${pkgs.findutils}/bin/find src -type f -name '*.c' -printf '%h\n' | ${pkgs.gnused}/bin/sed 's#^src/##' | ${pkgs.coreutils}/bin/sort -u)"

  if [ -z "$targets" ]; then
    echo "No C targets found under src/. Skipping build check."
    mkdir -p "$out"
    touch "$out/passed"
    exit 0
  fi

  while IFS= read -r target; do
    [ -z "$target" ] && continue
    echo "Checking build for: $target"
    build_dir="build-check/$target"
    ${pkgs.coreutils}/bin/mkdir -p "$build_dir"
    ${pkgs.cmake}/bin/cmake -S . -B "$build_dir" -DTARGET="$target" -DCMAKE_BUILD_TYPE=Release
    ${pkgs.cmake}/bin/cmake --build "$build_dir"
  done <<EOF
  $targets
  EOF

  mkdir -p "$out"
  touch "$out/passed"
''
