{...}: {
  perSystem = {
    pkgs,
    config,
    ...
  }: {
    packages = import ./packages {inherit pkgs;};

    formatter = pkgs.alejandra;

    devShells.default = pkgs.mkShell {
      packages = [
        pkgs.gcc
        config.packages.format-all
        config.packages.format-check
        config.packages.test-all
      ];
    };
  };
}
