//source file
#include "../include/large_numerical_container_initializer_677.h"

// ========================================================================================================
#define MUT677_OUTPUT 1

void MutatorFrontendAction_677::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ContainerInit")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      // Get the source code text of target node
      auto constructorDecl = CT->getConstructor();
      if (!constructorDecl || !constructorDecl->isTemplateInstantiation())
        return;

      if (CT->getNumArgs() >= 2) {
        if (auto *sizeArg = llvm::dyn_cast<clang::IntegerLiteral>(CT->getArg(0))) {
          //Perform mutation on the source code text by applying string replacement
          llvm::APInt largeSize(64, 1099511627776); // 2^40
          auto sizeArgStr = std::to_string(largeSize.getZExtValue());
          Rewrite.ReplaceText(sizeArg->getSourceRange(), sizeArgStr);
        }
      }
    }
}
  
void MutatorFrontendAction_677::MutatorASTConsumer_677::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxConstructExpr(hasDeclaration(cxxConstructorDecl(ofClass(hasAnyName("std::vector", "std::list")))),
                                    hasArgument(0, integerLiteral())).bind("ContainerInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}