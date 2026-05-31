//source file
#include "../include/modify_default_parameters_in_constructors_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;

      for (auto *Param : Ctor->parameters()) {
        if (Param->hasDefaultArg()) {
          auto DefaultArgRange = Param->getDefaultArgRange();
          //Get the source code text of target node
          auto DefaultArgText = stringutils::rangetoStr(*(Result.SourceManager),
                                                        DefaultArgRange);
          //Perform mutation on the source code text by applying string replacement
          size_t equalPos = DefaultArgText.find('=');
          if (equalPos != std::string::npos) {
            DefaultArgText = DefaultArgText.substr(0, equalPos + 1);
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(DefaultArgRange), DefaultArgText);
        }
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArg())).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}