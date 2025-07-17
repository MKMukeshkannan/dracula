{
  description = "A Nix-flake-based C/C++ development environment";
  inputs.barracks.url = "path:/Users/mukeshkannan/mk/dev/barracks";

  outputs = { nixpkgs, barracks, ... }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
        rapidcsv = barracks.packages.${system}.rapidcsv;
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs, rapidcsv  }: {
        default = pkgs.mkShell {
            packages = with pkgs; [
              clang-tools
              cmake
              gnumake

              bear

              rapidcsv
              (imgui.override { IMGUI_BUILD_GLFW_BINDING = true; })
              implot
              glfw3
              glew
            ];
          };
      });
    };
}
