{
  stdenv,
  lib,
  ncurses,
}:
stdenv.mkDerivation {
  name = "plazza";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ncurses]; # used for make logs

  enableParallelBuilding = true;
  makeFlags = ["all"];

  installPhase = ''
    runHook preInstall

    install -Dm 755 plazza -t $out/bin

    runHook postInstall
  '';

  meta = {
    description = "Simulate a working pizzera with multiple processs and threads";
    license = "Apache NON-IA License 2.0"; # variant on apache-2.0
    maintainer = with lib.maintainers; [sigmanifiicient];
    mainProgram = "plazza";
  };
}
