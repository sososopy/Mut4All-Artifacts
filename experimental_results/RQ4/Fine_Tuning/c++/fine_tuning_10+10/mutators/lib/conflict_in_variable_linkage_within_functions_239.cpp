//source file
#include "../include/Conflict_In_Variable_Linkage_Within_Functions_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("variable")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        
        // Ensure the variable is declared within a function
        if (VD->getDeclContext()->isFunctionOrMethod()) {
            //Get the source code text of target node
            auto varName = VD->getNameAsString();
            auto varType = VD->getType().getAsString();
            
            //Perform mutation on the source code text by applying string replacement
            std::string externDecl = "\nextern " + varType + " " + varName + ";";
            //Replace the original AST node with the mutated one
            Rewrite.InsertTextAfterToken(VD->getEndLoc(), externDecl);
        }
    }
}
  
void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(unless(parmVarDecl())).bind("variable");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}