//source file
#include "../include/simplify_array_size_expression_606.h"

// ========================================================================================================
#define MUT606_OUTPUT 1

void MutatorFrontendAction_606::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *CAT = llvm::dyn_cast<clang::ConstantArrayType>(VD->getType())) {
        //Get the source code text of target node
        auto arraySizeExpr = CAT->getSizeExpr();
        if (!arraySizeExpr)
          return;

        //Perform mutation on the source code text by applying string replacement
        std::string mutatedSize = "2"; // Simplified constant size
        Rewrite.ReplaceText(arraySizeExpr->getSourceRange(), mutatedSize);
      }
    }
}
  
void MutatorFrontendAction_606::MutatorASTConsumer_606::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(constantArrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}