{pkgs}: let
  formatNix =
    pkgs.runCommand "format-nix-check" {
      src = ../../.;
      nativeBuildInputs = [
        pkgs.alejandra
        pkgs.coreutils
      ];
    } ''
      cp -r "$src" source
      chmod -R +w source
      cd source

      ${pkgs.alejandra}/bin/alejandra --check .

      mkdir -p "$out"
      touch "$out/passed"
    '';

  formatC =
    pkgs.runCommand "format-c-check" {
      src = ../../.;
      nativeBuildInputs = [
        pkgs.clang-tools
        pkgs.coreutils
        pkgs.findutils
      ];
    } ''
      cp -r "$src" source
      chmod -R +w source
      cd source

      files="$(${pkgs.findutils}/bin/find src -type f \( -name '*.c' -o -name '*.h' \) | ${pkgs.coreutils}/bin/sort)"

      if [ -z "$files" ]; then
        echo "No C files found under src/. Skipping C format check."
        mkdir -p "$out"
        touch "$out/passed"
        exit 0
      fi

      while IFS= read -r file; do
        [ -z "$file" ] && continue
        ${pkgs.clang-tools}/bin/clang-format --dry-run --Werror "$file"
      done <<EOF
      $files
      EOF

      mkdir -p "$out"
      touch "$out/passed"
    '';
in {
  format-nix = formatNix;
  format-c = formatC;
}
