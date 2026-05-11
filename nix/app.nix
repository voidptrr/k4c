{pkgs}: let
  runner = pkgs.writeShellApplication {
    name = "run-dsa";
    runtimeInputs = [
      pkgs.cmake
      pkgs.gcc
      pkgs.coreutils
    ];
    text = ''
      set -eu

      if [ "$#" -lt 1 ]; then
        echo "Usage: nix run .#run -- <target> [program args...]"
        echo "Example: nix run .#run -- datastruct/array"
        exit 1
      fi

      target="$1"
      shift

      target_dir="src/$target"
      if [ ! -d "$target_dir" ]; then
        echo "Error: missing target directory: $target_dir"
        exit 1
      fi

      set -- "$target_dir"/*.c "$@"
      if [ "$1" = "$target_dir/*.c" ]; then
        echo "Error: no .c files found in: $target_dir"
        echo "Create at least one C source file, then run again."
        exit 1
      fi
      shift

      build_dir="build/$target"
      mkdir -p "$build_dir"
      binary_name="$(printf '%s' "$target" | tr '/' '-')"
      output_binary="$build_dir/$binary_name"

      echo "Configuring CMake for $target"
      cmake -S . -B "$build_dir" -DTARGET="$target" -DCMAKE_BUILD_TYPE=Release

      echo "Building $target"
      cmake --build "$build_dir"

      echo "Running $output_binary"
      "$output_binary" "$@"
    '';
  };
in {
  type = "app";
  program = "${runner}/bin/run-dsa";
}
