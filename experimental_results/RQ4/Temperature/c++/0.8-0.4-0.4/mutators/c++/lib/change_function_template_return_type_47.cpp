//source file
#include "../include/change_function_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto StartLoc = FTD->getTemplatedDecl()->getReturnTypeSourceRange().getBegin();
      auto EndLoc = FTD->getTemplatedDecl()->getReturnTypeSourceRange().getEnd();

      if (StartLoc.isValid() && EndLoc.isValid()) {
        //Perform mutation on the source code text by applying string replacement
        std::string ReturnTypeText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts).str();
        if (ReturnTypeText == "auto") {
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), "decltype(auto)");
        }
      }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(functionDecl(returns(autoType())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}