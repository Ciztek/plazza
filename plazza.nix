{
  stdenv,
  makeWrapper,
  lib,
  sfml,
}:
stdenv.mkDerivation {
  name = "plazza";
  version = "0.1.0";

  src = ./.;

  enableParallelBuilding = true;
  makeFlags = ["all"];

  nativeBuildInputs = [makeWrapper];

  installPhase = ''
    install -Dm 755 plazza -t $out/bin
  '';

  meta = {
    description = "WHO SAID ANYTHING ABOUT PIZZAS?";
    mainProgram = "plazza";
  };
}
