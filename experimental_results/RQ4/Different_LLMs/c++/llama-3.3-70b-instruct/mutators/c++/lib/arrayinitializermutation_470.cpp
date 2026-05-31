//source file
#include "../include/ArrayInitializerMutation_470.h"

// ========================================================================================================
#define MUT470_OUTPUT 1

void MutatorFrontendAction_470::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (VD->getType()->isArrayType()) {
        std::string mutatedDeclaration = declaration;
        // Replace array initialization with pointer initialization
        size_t pos = mutatedDeclaration.find("=");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 1, "*");
        }
        // Replace array type with pointer type
        pos = mutatedDeclaration.find("[");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 1, "*");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_470::MutatorASTConsumer_470::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}