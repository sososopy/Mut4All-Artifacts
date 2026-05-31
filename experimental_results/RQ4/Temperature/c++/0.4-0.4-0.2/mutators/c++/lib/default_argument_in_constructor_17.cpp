//source file
#include "../include/Default_Argument_In_Constructor_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      // Ensure the constructor has parameters
      if (CT->getNumParams() > 0) {
        // Get the source code text of target node
        auto param = CT->getParamDecl(0);
        if (param->getType()->isBuiltinType()) {
          // Perform mutation on the source code text by applying string replacement
          auto paramText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   param->getSourceRange());
          paramText += " = "; // Introduce a default argument without specifying a value
          Rewrite.ReplaceText(param->getSourceRange(), paramText);
        }
      }
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isDefinition()).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}