//source file
#include "../include/remove_default_argument_from_constructor_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      for (auto param : Ctor->parameters()) {
        if (param->hasDefaultArg() && param->getType()->isFundamentalType()) {
          auto defaultArgRange = param->getDefaultArgRange();
          Rewrite.RemoveText(defaultArgRange);
        }
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArgument())).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}