PROGRAMA QUE AUXILIA NO CONTROLE DOS PROGRAS QUÂNTICOS

-> input do gamess sendo montado corretamente.
-> falta ler o output do gamess.
-> quem sabe trocar os coeficientes da base.

-> LENDO OUTPUT DO GAMESS
   -> falta implementar: read frequency.
                         read coordinates.
                         read gradient.


"quantumOptions.inp" - arquivo que contêm as opcoes.


USANDO: WriteQuantumInput
        [Objeto que constrói um input para um programa
         quântico usando um conjunto de coordenadas]

  -> CONSTRUCTOR

  WriteQuantumInput(
  vector<string> options ->
  options[0] = type: "mopac" ou "mopac2009"
  options[1] = projectName

  para "mopac" ou "mopac2009" temos
  options[2] = "cabecalho do mopac"
  options[3] = "nome dos arquivos externos"
  options[4] = "nome do átomo central"
  )

  -> CRIACAO DE INPUT

  string createInput(
  vector<CoordXYZ> coordinates
  sao as coordenadas que deseja-se adicionar ao input.

  int index
  numero a ser adicionado no final, serve para manter
  os inputs caso faça-se um grande número de contas.
  )

  retorna uma string com o nome do input.
  observacao -> para executa-la e necessario adicionar
  a extensao correspondente ".mop" para mopac.

  -> FUNÇÕES PÚBLICAS

  changeProjectName(
  string newProjectName
  todos os futuros inputs criados com esse objeto terao o novo
  nome designado aqui.
  )

  changeMopacHeader(
  string newMopacHeader
  todos os futuros inputs criados terao o cabeçalho definido aqui
  util para calcular frequencia, por exemplo.
  )


USANDO: ReadQuantumOutput
        [objeto que lê as propriedades de output quântico]

  CONSTRUCTOR
  -> ReadQuantumOutput(
  string type
  "mopac" ou "mopac2009"
  )

  LEITURA DO OUTPUT
  -> readOutput(
  string inputName
  nome do output a ser lido. Pode ser a mesma saída do
  WriteQuantumInput::createInput
  )

  observação1: a leitura usa sempre a extensao ".out"

  observação2: sempre tenta ler tudo com  os criterios definidos
               em ReadQuantumOutput, o que ele achar, é isso mesmo.

  observação3: as informacoes ficam dentro do objeto

  PÚBLICAS
 
  vector<CoordXYZ> getCoordinates()
  double getEnergy()
  double getIonizationPotential()
  vector<double> getDipole()
  double getFirstFrequency()


IMPLEMENTAÇÂO

Funcoes que precisam ser implementadas:

WriteQuantumInput::createInput
WriteQuantumInput::setInputProperties

ReadQuantumOutput() constructor
-> como ler
ReadQuantumOutput::readCoordinates
ReadQuantumOutput::readEnergy
ReadQuantumOutput::readIonization
ReadQuantumOutput::readDipole
ReadQuantumOutput::readFrequency
-> quando ler
ReadQuantumOutput::haveToReadCoordinates
ReadQuantumOutput::haveToReadEnergy
ReadQuantumOutput::haveToReadIonization
ReadQuantumOutput::haveToReadDipole
ReadQuantumOutput::haveToReadFrequency







