{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
	buildInputs = with pkgs; [
		glfw
		wayland
		wayland-protocols
		libxkbcommon

		libGL
		libGLU
	];

	shellHook = ''
		export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${pkgs.lib.makeLibraryPath (with pkgs; [ libGL ])}"
		echo "--- NixOS Development Environment Loaded ---"
		echo "Compiler: $(g++ --version | head -n1)"
	'';
}
