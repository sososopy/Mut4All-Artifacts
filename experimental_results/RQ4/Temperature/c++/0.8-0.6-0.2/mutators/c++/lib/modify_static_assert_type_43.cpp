//source file
#include "../include/modify_static_assert_type_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;

      //Get the source code text of target node
      auto assertionExpr = SA->getAssertExpr();
      if (auto *CallExpr = llvm::dyn_cast<clang::CallExpr>(assertionExpr)) {
        if (CallExpr->getNumArgs() == 2) {
          //Perform mutation on the source code text by applying string replacement
          auto arg1 = CallExpr->getArg(0);
          auto arg2 = CallExpr->getArg(1);
          if (auto *BuiltinCall = llvm::dyn_cast<clang::CallExpr>(arg1)) {
            if (BuiltinCall->getDirectCallee()->getName() == "__builtin_types_compatible_p") {
              auto typeArg = BuiltinCall->getArg(1);
              //Replace the original AST node with the mutated one
              std::string mutatedText = "!(__builtin_types_compatible_p(typeof(*var), float))";
              Rewrite.ReplaceText(typeArg->getSourceRange(), "float");
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = staticAssertDecl().bind("StaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}