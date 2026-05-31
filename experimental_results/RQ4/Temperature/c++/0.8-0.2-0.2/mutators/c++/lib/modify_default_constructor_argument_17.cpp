//source file
#include "../include/modify_default_constructor_argument_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("CtorWithDefaultArg")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;
      
      for (auto *Param : Ctor->parameters()) {
        if (Param->hasDefaultArg()) {
          auto DefaultArg = Param->getDefaultArg();
          if (auto *IntLit = dyn_cast<IntegerLiteral>(DefaultArg)) {
            //Perform mutation on the source code text by applying string replacement
            auto Replacement = llvm::Twine(IntLit->getValue().getSExtValue()) + ".0f";
            Rewrite.ReplaceText(DefaultArg->getSourceRange(), Replacement.str());
          }
        }
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArgument())).bind("CtorWithDefaultArg");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}