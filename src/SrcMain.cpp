#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>

extern int proccparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void procc_flush_buffer(yy_buffer_state* b); // NOLINT
extern int procclex_destroy(); // NOLINT
extern FILE* proccin; // NOLINT

extern int gLineNumber;
extern NProgram* gProgram;
bool gSyntaxError = false;

// CHANGE ANYTHING ABOVE THIS LINE AT YOUR OWN RISK!!!!


int ProcessCommandArgs(int argc, const char* argv[])
{
	gLineNumber = 1;
	gSyntaxError = false;
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return 1;
	}

	// Read input from the first command line parameter
	proccin = fopen(argv[1], "r");
	if (proccin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return 1;
	}
	procc_flush_buffer(nullptr);

	// Start the parse
	proccparse();

	if (gProgram != nullptr && argc == 3)
	{
		// TODO: Add any needed code for Parts 2, 3, and 4!!!
        //create the ast file and create AST
        std::ofstream oFile("ast.txt");
        gProgram->OutputAST(oFile, 0);
        //oFile.close();
        std::string arg = (std::string)argv[2];
        if(arg == "emit"){
            CodeContext c;
            //generate the code
            gProgram->CodeGen(c);
            //print out codecontext
            std::ofstream oFile1("emit.txt");
            for(auto elem:c.opList){
                oFile1 << std::get<0>(elem) << " ";
                if(!std::get<1>(elem).empty()){
                    oFile1 << std::get<1>(elem);
                }
                if(!std::get<2>(elem).empty()){
                    oFile1 << "," << std::get<2>(elem);
                }
                if(!std::get<3>(elem).empty()){
                    oFile1 << "," << std::get<3>(elem);
                }
                oFile1 << std::endl;
            }
            //oFile1.close();
            
        }
        arg = (std::string)argv[2];
        if(arg == "reg"){
            CodeContext c;
            gProgram->CodeGen(c);
//******************************interval generation***********************
            std::string tempReg;
            std::map<std::string,std::pair<int,int>> intrMap;
            std::tuple<std::string,std::string,std::string,std::string> elem;
            for(int i = 0; i<c.opList.size();i++){
                elem = c.opList[i];
                //update interval map for each parameter in op list
                tempReg = std::get<1>(elem);
                if(tempReg.substr(0,1) == "%"){
                    if(static_cast<bool>(intrMap.count(tempReg))){
                        //update end value
                        intrMap[tempReg].second = i;
                    }else{
                        //create pair for a new virtual register
                        intrMap[tempReg] = std::pair<int,int>(i,i);
                    }
                }
                tempReg = std::get<2>(elem);
                if(tempReg.substr(0,1) == "%"){
                    if(static_cast<bool>(intrMap.count(tempReg))){
                        //update end value
                        intrMap[tempReg].second = i;
                    }else{
                        //create pair for a new virtual register
                        intrMap[tempReg] = std::pair<int,int>(i,i);
                    }
                }
                tempReg = std::get<3>(elem);
                if(tempReg.substr(0,1) == "%"){
                    if(static_cast<bool>(intrMap.count(tempReg))){
                        //update end value
                        intrMap[tempReg].second = i;
                    }else{
                        //create pair for a new virtual register
                        intrMap[tempReg] = std::pair<int,int>(i,i);
                    }
                }
            }
//******************************interval print****************************
            std::ofstream intrFile("reg.txt");
            intrFile << "INTERVALS:\n";
            for(int i = 0; i < intrMap.size();i++){
                tempReg = "%" + std::to_string(i);
                auto itr = intrMap.find(tempReg);
                intrFile << tempReg << ":" << itr->second.first << "," << itr->second.second << "\n";
            }
//******************************allocation********************************
            //vector with indices representing the r0 to r7
            std::vector<bool> realReg = {true,true,true,true,true,true,true};
            //map of virtual to real registers
            std::vector<std::string> vrList(intrMap.size());
            //linear scan
            for(int i = 0; i < c.opList.size(); i++){
                for(int j = 0; j < vrList.size();j++){
                    //capture interval information
                    auto temp = intrMap.find("%"+std::to_string(j));
                    //update real register list when a virutal register is found inactive
                    if(temp->second.second == i){
                        realReg[std::stoi(vrList[std::stoi(temp->first.substr(1))].substr(1))-1] = true;
                    }
                    if(temp->second.first == i){
                        for(int k = 0; k < realReg.size();k++){
                            //look for the first aavailable real register and update vrList
                            if(realReg[k]){
                                vrList[j] = "r" + std::to_string(k+1);
                                realReg[k] = false;
                                break;
                            }
                        }
                    }
                }
            }
//******************************allocation print****************************
            intrFile << "ALLOCATION:\n";
            for(int i = 0; i < vrList.size();i++){
                tempReg = "%" + std::to_string(i);
                intrFile << tempReg << ":" << vrList[i] << "\n";
            }
//******************************Replace virtual with real Reg****************************
            //replacement scan
            for(auto& tempTuple:c.opList){
                tempReg = std::get<1>(tempTuple);
                if(tempReg.substr(0,1) == "%"){
                    std::get<1>(tempTuple) = vrList[std::stoi(tempReg.substr(1))];
                }
                tempReg = std::get<2>(tempTuple);
                if(tempReg.substr(0,1) == "%"){
                    std::get<2>(tempTuple) = vrList[std::stoi(tempReg.substr(1))];
                }
                tempReg = std::get<3>(tempTuple);
                if(tempReg.substr(0,1) == "%"){
                    std::get<3>(tempTuple) = vrList[std::stoi(tempReg.substr(1))];
                }
            }
//***************************print final code*******************************
            std::ofstream oFile1("emit.txt");
            for(auto elem:c.opList){
                oFile1 << std::get<0>(elem) << " ";
                if(!std::get<1>(elem).empty()){
                    oFile1 << std::get<1>(elem);
                }
                if(!std::get<2>(elem).empty()){
                    oFile1 << "," << std::get<2>(elem);
                }
                if(!std::get<3>(elem).empty()){
                    oFile1 << "," << std::get<3>(elem);
                }
                oFile1 << std::endl;
            }
//***************************end of edited area*******************************
        }
	}
	else
	{
		// (Just a useful separator for debug cout statements in grammar)
		std::cout << "**********************************************\n";
	}

	// Close the file stream
	fclose(proccin);
	// Destroy lexer so it reinitializes
	procclex_destroy();
	// Return 1 if syntax error, 0 otherwise
	return static_cast<int>(gSyntaxError);
}

void proccerror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
	gSyntaxError = true;
}
