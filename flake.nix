{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    pre-commit-hooks = {
      url = "github:cachix/git-hooks.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = {
    self,
    nixpkgs,
    pre-commit-hooks,
  }: let
    forAllSystems = function:
      nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ] (system: function nixpkgs.legacyPackages.${system});
  in {
    devShells = forAllSystems (pkgs: {
      default = pkgs.mkShell {
        inherit (self.checks.${pkgs.system}.pre-commit-check) shellHook;

        hardeningDisable = ["fortify"];
        inputsFrom = [self.packages.${pkgs.system}.plazza];
        packages = with pkgs; [
          clang-tools
          gcc
          gnumake
          compiledb
          gcovr
          pkg-config
        ];
      };
    });

    formatter = forAllSystems (pkgs: pkgs.alejandra);

    checks = forAllSystems (pkgs: let
      hooks = {
        alejandra.enable = true;
        trim-trailing-whitespace.enable = true;

        commit-name = {
          enable = true;
          name = "commit name";
          stages = ["commit-msg"];
          entry = ''
            ${pkgs.python310.interpreter} ${./check_commit_message.py}
          '';
        };
      };
    in {
      pre-commit-check = pre-commit-hooks.lib.${pkgs.system}.run {
        inherit hooks;
        src = ./.;
      };
    });

    packages = forAllSystems (pkgs: {
      default = self.packages.${pkgs.system}.plazza;

      plazza = pkgs.callPackage ./plazza.nix {};
    });
  };
}
