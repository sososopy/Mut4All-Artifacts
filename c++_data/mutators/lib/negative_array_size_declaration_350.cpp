//source file
#include "../include/Negative_Array_Size_Declaration_350.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *AT = dyn_cast<clang::ConstantArrayType>(VD->getType().getTypePtr())) {
        //Get the source code text of target node
        auto arraySizeExpr = AT->getSizeExpr();
        if (!arraySizeExpr)
          return;

        //Perform mutation on the source code text by applying string replacement
        std::string newSize = "-1"; // You can choose any negative integer
        Rewrite.ReplaceText(arraySizeExpr->getSourceRange(), newSize);
      }
    }
}
  
void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}