//source file
#include "../include/mix_struct_initialization_styles_in_new_expression_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      //Filter nodes in header files
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;
      if (!NE->isParenTypeId())
        return;
      if (NE->isArray())
        return;
      if (NE->getAllocatedType()->isStructureOrClassType() == false)
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             NE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto args = NE->getInitializer()->children();
      if (std::distance(args.begin(), args.end()) <= 1)
        return;
      llvm::outs() << "mut99: " << content << '\n';
      auto arg1 = stringutils::rangetoStr(*(Result.SourceManager),
                                          (*args.begin())->getSourceRange());
      auto arg2 = stringutils::rangetoStr(*(Result.SourceManager),
                                          (*(++args.begin()))->getSourceRange());
      llvm::outs() << arg1 << " " << arg2 << '\n';
      content.replace(content.find(arg2), arg2.size(), "{" + arg2 + "}");
      llvm::outs() << "mut99: " << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}