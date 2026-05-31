//source file
#include "../include/Add_Const_Generics_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      if (LT->getExplicitTemplateParameters().empty())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto tpl = LT->getExplicitTemplateParameters();
      auto tpl_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 tpl.front()->getSourceRange());
      tpl_content.insert(tpl_content.rfind('>'), ",auto&&...");
      llvm::outs() << tpl_content;
      content.replace(content.find('['), tpl_content.size(), tpl_content);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}