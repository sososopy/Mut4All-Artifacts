//source file
#include "../include/Replace_Empty_Initializer_List_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if the variable has an initializer
      const Expr *Init = VD->getInit();
      if (!Init) return;
      //Check if the initializer is an InitListExpr with empty list
      if (!isa<InitListExpr>(Init)) return;
      const InitListExpr *ILE = cast<InitListExpr>(Init);
      if (ILE->getNumInits() != 0) return;
      //Check if the declared type is an aggregate
      QualType QT = VD->getType();
      const Type *T = QT.getTypePtr();
      if (!T->isAggregateType()) return;
      //Check if the aggregate contains at least one non-aggregate member
      bool hasNonAggregateMember = false;
      if (const RecordType *RT = T->getAs<RecordType>()) {
        const RecordDecl *RD = RT->getDecl();
        if (RD->isCompleteDefinition()) {
          for (const FieldDecl *FD : RD->fields()) {
            QualType MemberQT = FD->getType();
            const Type *MemberT = MemberQT.getTypePtr();
            if (MemberT->isClassType()) {
              const CXXRecordDecl *CRD = MemberT->getAsCXXRecordDecl();
              if (CRD && CRD->hasUserDeclaredConstructor()) {
                hasNonAggregateMember = true;
                break;
              }
            }
            else if (!MemberT->isAggregateType()) {
              hasNonAggregateMember = true;
              break;
            }
          }
        }
      }
      else if (T->isArrayType()) {
        const ArrayType *AT = T->getAsArrayTypeUnsafe();
        QualType ElementQT = AT->getElementType();
        const Type *ElementT = ElementQT.getTypePtr();
        if (ElementT->isClassType()) {
          const CXXRecordDecl *CRD = ElementT->getAsCXXRecordDecl();
          if (CRD && CRD->hasUserDeclaredConstructor()) {
            hasNonAggregateMember = true;
          }
        }
        else if (!ElementT->isAggregateType()) {
          hasNonAggregateMember = true;
        }
      }
      if (!hasNonAggregateMember) return;
      //Get the source code text of target node
      SourceRange InitRange = Init->getSourceRange();
      std::string TypeStr = VD->getType().getAsString();
      //Perform mutation on the source code text by applying string replacement
      std::string Replacement = " = " + TypeStr + "()";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(InitRange, Replacement);
    }
}
  
void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}