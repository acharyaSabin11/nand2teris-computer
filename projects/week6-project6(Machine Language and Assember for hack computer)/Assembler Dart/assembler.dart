import 'dart:io';

const Map<String, String> instructionMap = {
  '0': '0101010',
  '1': '0111111',
  '-1': '0111010',
  'D': '0001100',
  'A': '0110000',
  'M': '1110000',
  '!D': '0001101',
  '!A': '0110001',
  '!M': '1110001',
  '-D': '0001111',
  '-A': '0110011',
  '-M': '1110011',
  'D+1': '0011111',
  'A+1': '0110111',
  'M+1': '1110111',
  'D-1': '0001110',
  'A-1': '0110010',
  'M-1': '1110010',
  'D+A': '0000010',
  'D+M': '1000010',
  'D-A': '0010011',
  'D-M': '1010011',
  'A-D': '0000111',
  'M-D': '1000111',
  'D&A': '0000000',
  'D&M': '1000000',
  'D|A': '0010101',
  'D|M': '1010101',
};

const Map<String, String> destinationMap = {
  'null': '000',
  'M': '001',
  'D': '010',
  'MD': '011',
  'A': '100',
  'AM': '101',
  'AD': '110',
  'AMD': '111',
};

// to store the jump values

const Map<String, String> jumpMap = {
  'null': '000',
  'JGT': '001',
  'JEQ': '010',
  'JGE': '011',
  'JLT': '100',
  'JNE': '101',
  'JLE': '110',
  'JMP': '111',
};

//similarly making a map and inserting above key value pairs

Map<String, String> symbolTable = {
  'R0': '000000000000000',
  'R1': '000000000000001',
  'R2': '000000000000010',
  'R3': '000000000000011',
  'R4': '000000000000100',
  'R5': '000000000000101',
  'R6': '000000000000110',
  'R7': '000000000000111',
  'R8': '000000000001000',
  'R9': '000000000001001',
  'R10': '000000000001010',
  'R11': '000000000001011',
  'R12': '000000000001100',
  'R13': '000000000001101',
  'R14': '000000000001110',
  'R15': '000000000001111',
};

void main() {
  //adding above defined key value pairs to the map

  final file = new File('./input.asm');
  final lines = file.readAsLinesSync();
  for (int i = 0; i < lines.length; i++) {
    codeExtractorAndValidator(lines[i], i + 1);
  }
}

void codeExtractorAndValidator(String line, int lineNumber) {
  if (line.length == 0) {
    return;
  }

  for (int i = 0; i < line.length; i++) {
    if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {}
  }
}
