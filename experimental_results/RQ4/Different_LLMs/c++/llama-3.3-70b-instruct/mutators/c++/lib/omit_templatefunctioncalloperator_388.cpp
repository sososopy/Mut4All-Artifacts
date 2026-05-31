//source file
#include "../include/Omit_TemplateFunctionCallOperator_388.h"

// ========================================================================================================
#define MUT388_OUTPUT 1

void MutatorFrontendAction_388::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getDirectCallee()) {
        auto callee = MT->getDirectCallee();
        if (callee->getKind() == Decl::Kind::FunctionTemplate) {
          auto funcTemplate = cast<FunctionTemplateDecl>(callee);
          if (funcTemplate->getTemplatedDecl()->getKind() == Decl::Kind::CXXMethod) {
            auto method = cast<CXXMethodDecl>(funcTemplate->getTemplatedDecl());
            if (method->isInstance() && !method->isStatic()) {
                // Omit the function call operator
                auto mutatedDeclaration = declaration.substr(0, declaration.find("("));
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
              }
            }
          }
        }
      }
}

void MutatorFrontendAction_388::MutatorASTConsumer_388::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}