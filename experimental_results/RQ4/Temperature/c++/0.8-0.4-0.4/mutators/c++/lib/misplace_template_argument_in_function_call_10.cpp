//source file
#include "../include/misplace_template_argument_in_function_call_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (auto *DRE = dyn_cast<DeclRefExpr>(CE->getCallee()->IgnoreImplicit())) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->isTemplated() && CE->getNumArgs() > 0) {
            //Get the source code text of target node
            auto callText = stringutils::rangetoStr(*(Result.SourceManager),
                                                    CE->getSourceRange());

            //Perform mutation on the source code text by applying string replacement
            std::string mutatedCallText = callText;
            if (CE->getNumArgs() > 1) {
              // Misplace template argument by adding an extra one
              mutatedCallText.insert(mutatedCallText.find(">"), ", double");
            } else {
              // Remove the existing template argument
              size_t start = mutatedCallText.find("<");
              size_t end = mutatedCallText.find(">");
              if (start != std::string::npos && end != std::string::npos) {
                mutatedCallText.erase(start, end - start + 1);
              }
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCallText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}