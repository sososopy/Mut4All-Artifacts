//source file
#include "../include/GlobalVariableToModuleVariable_81.h"

// ========================================================================================================
#define MUT81_OUTPUT 1

void MutatorFrontendAction_81::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VarDecl->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VarDecl->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find_first_not_of(" \t");
      if (pos != string::npos) {
        declaration.insert(pos, "module ");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarDecl->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_81::MutatorASTConsumer_81::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}