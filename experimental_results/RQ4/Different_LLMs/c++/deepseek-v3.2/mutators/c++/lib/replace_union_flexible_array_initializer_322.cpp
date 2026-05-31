//source file
#include "../include/Mutator_322.h"

// ========================================================================================================
#define MUT322_OUTPUT 1

void MutatorFrontendAction_322::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("UnionVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if the variable is a union type
      const Type *T = VD->getType().getTypePtr();
      if (!T || !T->isUnionType())
        return;
      //Check if the union has an initializer
      const Expr *Init = VD->getInit();
      if (!Init)
        return;
      //Check if the union contains a flexible array member
      const RecordDecl *RD = T->getAsRecordDecl();
      if (!RD)
        return;
      bool hasFlexibleArrayMember = false;
      for (const FieldDecl *FD : RD->fields()) {
        if (FD->getType()->isArrayType() && FD->getType()->getAsArrayTypeUnsafe()->getSizeModifier() == ArrayType::Normal) {
          const ConstantArrayType *CAT = dyn_cast<ConstantArrayType>(FD->getType()->getAsArrayTypeUnsafe());
          if (!CAT || CAT->getSize().isZero()) {
            hasFlexibleArrayMember = true;
            break;
          }
        }
      }
      if (!hasFlexibleArrayMember)
        return;
      //Get the source code text of target node
      SourceRange InitRange = Init->getSourceRange();
      //Perform mutation on the source code text by applying string replacement
      //Replace the initializer with {}
      Rewrite.ReplaceText(InitRange, "{}");
    }
}
  
void MutatorFrontendAction_322::MutatorASTConsumer_322::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(expr())).bind("UnionVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}