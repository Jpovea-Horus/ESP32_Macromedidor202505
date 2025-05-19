#include "lib/converters.h"

String NameVar;
void NameVariableModbus(String NameV) {
  String sud_Namev = NameV.substring(4, 8);
  int typecase = hexToDec(sud_Namev);
  switch (typecase) {
    case 0: //0
      NameVar = "VFA";
      break;
    case 2: //1
      NameVar = "VFB";
      break;
    case 4: //2
      NameVar = "VFC";
      break;
    case 6: //3
      NameVar = "CFA";
      break;
    case 8: //4
      NameVar = "CFB";
      break;
    case 10: //5
      NameVar = "CFC";
      break; 
    case 12: //6
      NameVar = "PAFA";
      break;
    case 14: //7
      NameVar = "PAFB";
      break;
    case 16: //8
      NameVar = "PAFC";
      break;
    case 30: //9
      NameVar = "FPFA";
      break;
    case 32: //10
      NameVar = "FPFB";
      break;
    case 34: //11
      NameVar = "FPFC";
      break;
    case 52: //12
      NameVar = "TSE";
      break;
    case 56: //13
      NameVar = "TSVa";
      break;
    case 60: //14
      NameVar = "TSVar";
      break;
    case 62: //15
      NameVar = "TFT";
      break;
    case 70: //16
      NameVar = "FHz";
      break;
    case 72: //17
      NameVar = "TImKwh";
      break;
    case 74: //18
      NameVar = "TExKwh";
      break;
    case 76: //19
      NameVar = "TImKVarh";
      break;
    case 78: //20
      NameVar = "TExKVarh";
      break;
    case 80: //21
      NameVar = "TVah";
      break;
    case 82: //22
      NameVar = "Ah";
      break;
    case 200: //23
      NameVar = "VFAFB";
      break;
    case 202: //24
      NameVar = "VFBFC";
      break;
    case 204: //25
      NameVar = "VFCFA";
      break;
    case 224: //26
      NameVar = "NC";
      break;
    case 234: //27
      NameVar = "FALNVTHD";
      break;
    case 236: //28
      NameVar = "FBLNVTHD";
      break;
    case 238: //29
      NameVar = "FCLNVTHD";
      break;
    case 240: //30
      NameVar = "FACTHD";
      break;
    case 242: //31
      NameVar = "FBCTHD";
      break;
    case 244: //32
      NameVar = "FCCTHD";
      break;
    case 342: //33
      NameVar = "TKWh";
      break;
  }
}
