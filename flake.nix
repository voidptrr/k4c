{
  description = "C algorithms and data structures study workspace";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];

      perSystem = {pkgs, ...}: {
        devShells.default = import ./nix/shell.nix {inherit pkgs;};

        formatter = pkgs.alejandra;

        apps = {
          run = import ./nix/app.nix {inherit pkgs;};
          format = import ./nix/format.nix {inherit pkgs;};
        };

        checks = import ./nix/checks {inherit pkgs;};
      };
    };
}
