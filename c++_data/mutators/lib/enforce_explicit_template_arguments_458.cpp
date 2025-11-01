//source file
#include "../include/Enforce_Explicit_Template_Arguments_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MethodCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      if (auto *MethodDecl = MT->getMethodDecl()) {
        if (MethodDecl->getDescribedFunctionTemplate()) {
          //Get the source code text of target node
          auto callExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          if (MT->getNumArgs() == 0) {
            auto templateArgs = MethodDecl->getTemplateSpecializationArgs();
            if (templateArgs && templateArgs->size() > 0) {
              std::string explicitArgs;
              for (unsigned i = 0; i < templateArgs->size(); ++i) {
                if (i > 0) explicitArgs += ", ";
                explicitArgs += templateArgs->get(i).getAsType().getAsString();
              }
              callExpr.insert(callExpr.find("()"), "<" + explicitArgs + ">");
            }
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), callExpr);
        }
      }
    }
}
  
void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxMemberCallExpr(callee(cxxMethodDecl(hasDescendant(templateArgumentLoc())))).bind("MethodCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}