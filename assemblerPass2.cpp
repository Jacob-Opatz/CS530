//
// Created by Jacob Opatz   on 10/31/23.
//
#include "sourceLineStruct.h"
#include "locationCounter.h"
#include "opcodeHandler.h"
#include "symbolTable.h"

#include "textRecordLineStruct.h"
#include "objectCodeLineStruct.h"
#include <string>
#include <vector>
#include <iostream>

std::vector<sourceLineStruct> pass2(std::vector<sourceLineStruct> P1listingFile, SymbolTable pass1SymTab) {


    SymbolTable symtab = pass1SymTab;
    std::vector<sourceLineStruct> listingFile = P1listingFile;
    //first line of source file
    sourceLineStruct firstLine = listingFile[0];
    //first line of our object code
    textRecordLine firstObjLine;

    //check if first opcode == 'START'
    if(firstLine.operation!="START"){
        std::cout<<"Program not started with correct opcode: Start. \n Incorrect opcode: "+firstLine.operation + "\n";
        //throw error, program not started correctly
        exit(3);
    };

    //set defaults based on first line contents
    LocationCounter locctr = LocationCounter(firstLine.targetAddress);
    firstLine.lineAddress = locctr.getLocationCounter();
    firstObjLine.setNewObjLine("H", firstLine);
   //write first line
    listingFile.push_back(firstLine);

    //initialize text record
    textRecordLine textRecLine;
    std::vector<textRecordLine> textRecord;
    std::string operandAddress;
    std::string tempObjectCode;
    textRecLine.setNewObjLine("T", firstLine);
    sourceLineStruct currentLine = sourceLineStruct();

    //iterate through instructions
    for (int i = 1; currentLine.operation != "END"; i++) {
        //get next line

        currentLine = listingFile[i];
        //if not comment line
        if(currentLine.operation[0] != '#') {

            // if opcode = "BYTE" or "WORD"
            //TODO: process other directives, use assemblerDirectivesPass1.cpp
            if (currentLine.operation == "BYTE") {
                //convert constant to object code
                tempObjectCode = toHex(currentLine.targetAddress, 2);
            }
            else if (currentLine.operation == "WORD") {

                tempObjectCode = toHex(currentLine.targetAddress, 6);
            }
            //if opcode not byte or word, check opcode table for operation
            else if(checkOpcode(currentLine.operation)) {

                //if symbol in operand field
                if((currentLine.targetAddress[0] >= 64) && (currentLine.targetAddress[0] <= 122 )) {
                        //store symbol value in operand field
                        currentLine.targetAddress = symtab.getSymbolValue(currentLine.targetAddress);

                }

                else if (currentLine.targetAddress[0] == '='){
                    if (currentLine.label != "") {
                        currentLine.targetAddress = symtab.getLiteralAddress(currentLine.label);
                    }
                    else{
                        currentLine.targetAddress = symtab.getLiteralAddress(currentLine.targetAddress);
                    }
                }

                else {
                    //FIXME: throw error here, unless # or @
                    operandAddress = "000000";
                };
                //FIXME: finish assemble function in objectCodeLine.h - Jacob
                tempObjectCode = assemble(currentLine, symtab);


            };

             //check if objectCodeLine will fit in Text record
             //FIXME: extra credit
//             if(textRecLine.getLength() + tempObjectCode.length() > 69) {
//                 //write text record to object program
//                 textRecLine.recordLength = toHex(textRecLine.getLength(), 2);
//                 locctr.incrementLocationCounter(textRecLine.getLength());
//                 textRecord.push_back(textRecLine);
//                 //initialize new text record
//                 currentLine.getLineComponents(sourceLines[i + 1]);
//             }
//            //add object code to text record
//            textRecLine.objectCode.append(tempObjectCode);
            //add to listing file
            listingFile[i].hexInstruction = tempObjectCode;
//            listingFile.push_back(currentLine);



        }

        }

        return listingFile;

    }







