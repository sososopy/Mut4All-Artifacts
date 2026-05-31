//source file
#include "../include/invalidate_default_parameter_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("Param")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto param = stringutils::rangetoStr(*(Result.SourceManager),
                                           MT->getSourceRange());
      if (MT->hasDefaultArg() == false)
        return;
      //Perform mutation on the source code text by applying string replacement
      auto default_arg = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getDefaultArgRange());
      llvm::outs() << default_arg << '\n';
      auto pos = param.rfind(default_arg);
      if (pos != string::npos)
        param.replace(pos, default_arg.length(), "");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), param);
    }
}
  
void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parmVarDecl().bind("Param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}