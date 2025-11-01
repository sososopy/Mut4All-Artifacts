//source file
#include "../include/Modify_Template_Function_Return_Type_374.h"

// ========================================================================================================
#define MUT374_OUTPUT 1

void MutatorFrontendAction_374::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || !FD->getReturnType()->isDependentType())
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedReturnType = "decltype(*T)";
      std::string originalReturnType = "auto";
      size_t pos = declaration.find(originalReturnType);
      if (pos != std::string::npos) {
        declaration.replace(pos, originalReturnType.length(), mutatedReturnType);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_374::MutatorASTConsumer_374::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(),
                                              returns(autoType())).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}