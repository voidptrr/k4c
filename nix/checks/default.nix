{pkgs}: let
  formatChecks = import ./format.nix {inherit pkgs;};
in
  {
    build-all = import ./build.nix {inherit pkgs;};
  }
  // formatChecks
