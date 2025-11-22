/**
 * SVFIR.cpp
 * @author kisslune
 */

#include "Graphs/SVFG.h"
#include "SVF-LLVM/SVFIRBuilder.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/Path.h"

using namespace SVF;
using namespace llvm;
using namespace std;

int main(int argc, char** argv)
{
    int arg_num = 0;
    int extraArgc = 4;
    char** arg_value = new char*[argc + extraArgc];
    for (; arg_num < argc; ++arg_num) {
        arg_value[arg_num] = argv[arg_num];
    }
    std::vector<std::string> moduleNameVec;

    int orgArgNum = arg_num;
    arg_value[arg_num++] = (char*)"-model-arrays=true";
    arg_value[arg_num++] = (char*)"-pre-field-sensitive=false";
    arg_value[arg_num++] = (char*)"-model-consts=true";
    arg_value[arg_num++] = (char*)"-stat=false";
    assert(arg_num == (orgArgNum + extraArgc) && "more extra arguments? Change the value of extraArgc");

    moduleNameVec = OptionBase::parseOptions(arg_num, arg_value, "SVF IR", "[options] <input-bitcode...>");

    LLVMModuleSet::getLLVMModuleSet()->buildSVFModule(moduleNameVec);

    // Instantiate an SVFIR builder
    SVFIRBuilder builder;
    cout << "Generating SVFIR(PAG), call graph and ICFG ..." << endl;

    // TODO: here, generate SVFIR(PAG), call graph and ICFG, and dump them to files
    //@{
    SVFIR* pag = builder.build();
    assert(pag && "Failed to build SVFIR");

    // Use the first input module name to decide where the .dot files go.
    llvm::SmallString<256> outputPrefix;
    if (!moduleNameVec.empty()) {
        outputPrefix = moduleNameVec.front();
        llvm::sys::path::remove_dots(outputPrefix, true);
        llvm::sys::path::replace_extension(outputPrefix, "");
    }
    if (outputPrefix.empty())
        outputPrefix = "svfir-output";

    const std::string baseName = std::string(outputPrefix.str());

    pag->dump(baseName + ".pag");
    CallGraph* callgraph = pag->getCallGraph();
    callgraph->dump(baseName + ".callgraph");
    ICFG* icfg = pag->getICFG();
    icfg->dump(baseName + ".icfg");
    //@}

    return 0;
}
