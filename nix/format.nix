{pkgs}: let
  formatter = pkgs.writeShellApplication {
    name = "format-all";
    runtimeInputs = [
      pkgs.alejandra
      pkgs.clang-tools
      pkgs.findutils
    ];
    text = ''
      set -eu

      echo "Formatting Nix files with alejandra"
      alejandra .

      echo "Formatting C files with clang-format"
      find src -type f \( -name '*.c' -o -name '*.h' \) -print0 | xargs -0 --no-run-if-empty clang-format -i

      echo "Formatting complete"
    '';
  };
in {
  type = "app";
  program = "${formatter}/bin/format-all";
}
