//source file
#include "../include/Mutator_TemplateArgumentDeductionRemoval_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NT = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!NT || !Result.Context->getSourceManager().isInMainFile(NT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   NT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string deduced_type = "deduced_type";
      size_t pos = declaration.find("(");
      declaration.insert(pos, "<" + deduced_type + ">");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}