//source file
#include "../include/Modify_Lambda_Param_Type_With_Decltype_Auto_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto lambda = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      auto params = LE->getLambdaClass()->getLambdaCallOperator()->parameters();
      for (auto param : params) {
        if (param->getType().getAsString() == "auto") {
          auto paramname = param->getNameAsString();
          auto sr = param->getSourceRange();
          auto paramtext = stringutils::rangetoStr(*(Result.SourceManager), sr);
          llvm::outs() << paramtext << '\n';
          llvm::outs() << paramname << '\n';
          auto pos = lambda.find(paramname);
          if (pos == string::npos)
            return;
          lambda.insert(pos, "decltype(auto)(");
          lambda.insert(pos + paramname.size() + 14, ")");
        }
      }
      lambda = "/*mut397*/" + lambda;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          lambda);
    }
}
  
void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}