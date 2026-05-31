//source file
#include "../include/Introduce_Constexpr_Incomplete_Struct_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    if (!namesCollected) {
        if (auto *ND = Result.Nodes.getNodeAs<clang::NamedDecl>("NamedDecl")) {
            if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
                return;
            if (ND->getIdentifier()) {
                existingNames.insert(ND->getNameAsString());
            }
        }
        return;
    }

    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("IncompleteStruct")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->getIdentifier())
            return;
        if (!RD->isCompleteDefinition()) {
            incompleteStructs.push_back(RD);
        }
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    Callback callback(TheRewriter);
    
    DeclarationMatcher nameMatcher = namedDecl().bind("NamedDecl");
    matchFinder.addMatcher(nameMatcher, &callback);
    matchFinder.matchAST(Context);
    
    callback.setNamesCollected(true);
    
    MatchFinder structFinder;
    DeclarationMatcher structMatcher = recordDecl().bind("IncompleteStruct");
    structFinder.addMatcher(structMatcher, &callback);
    structFinder.matchAST(Context);
    
    SourceManager &SM = TheRewriter.getSourceMgr();
    std::string newVarName = "mut_op_var_200";
    int counter = -1;
    do {
        counter++;
        newVarName = "mut_op_var_200";
        if (counter > 0) {
            newVarName += "_" + std::to_string(counter);
        }
    } while (callback.getExistingNames().find(newVarName) != callback.getExistingNames().end());
    
    std::string structName;
    bool needForwardDecl = false;
    
    if (!callback.getIncompleteStructs().empty()) {
        size_t idx = getrandom::getRandomIndex(callback.getIncompleteStructs().size() - 1);
        const RecordDecl* chosen = callback.getIncompleteStructs()[idx];
        structName = chosen->getNameAsString();
    } else {
        structName = "MutOpStruct_200";
        needForwardDecl = true;
    }
    
    std::string insertion;
    if (needForwardDecl) {
        insertion = "struct " + structName + ";\n";
    }
    insertion += "constexpr struct " + structName + " " + newVarName + " = { 0 };\n";
    insertion = "/*mut200*/" + insertion;
    
    SourceLocation insertLoc = SM.getLocForStartOfFile(SM.getMainFileID());
    TheRewriter.InsertText(insertLoc, insertion);
}