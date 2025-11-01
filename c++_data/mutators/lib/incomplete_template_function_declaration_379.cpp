//source file
#include "../include/Incomplete_Template_Function_Declaration_379.h"

// ========================================================================================================
#define MUT379_OUTPUT 1

void MutatorFrontendAction_379::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto templateParams = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FTD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string incompleteTemplate = templateParams + "\n// Incomplete function declaration\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), incompleteTemplate);
    }
}
  
void MutatorFrontendAction_379::MutatorASTConsumer_379::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}