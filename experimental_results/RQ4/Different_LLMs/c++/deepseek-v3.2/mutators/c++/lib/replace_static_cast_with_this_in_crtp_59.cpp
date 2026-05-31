//source file
#include "../include/Mutator_Replace_Static_Cast_With_This_In_CRTP_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXStaticCastExpr>("staticCastExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if this is a cast to a template parameter type (CRTP pattern)
      auto *CXXCE = dyn_cast<CXXStaticCastExpr>(CE);
      if (!CXXCE) return;
      
      QualType CastType = CXXCE->getTypeAsWritten();
      if (!CastType->isPointerType() && !CastType->isReferenceType()) return;
      
      // Get the derived type (strip pointer/reference)
      QualType DerivedType = CastType->getPointeeType();
      if (DerivedType.isNull()) DerivedType = CastType;
      
      // Check if the cast expression is *this or this
      const Expr *SubExpr = CXXCE->getSubExpr()->IgnoreImplicit();
      bool IsThisPointer = false;
      bool IsThisDeref = false;
      
      if (auto *UO = dyn_cast<UnaryOperator>(SubExpr)) {
        if (UO->getOpcode() == UO_Deref) {
          const Expr *UOSub = UO->getSubExpr()->IgnoreImplicit();
          if (auto *DRE = dyn_cast<DeclRefExpr>(UOSub)) {
            if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
              if (VD->getName() == "this") {
                IsThisDeref = true;
              }
            }
          }
        }
      } else if (auto *DRE = dyn_cast<DeclRefExpr>(SubExpr)) {
        if (auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          if (VD->getName() == "this") {
            IsThisPointer = true;
          }
        }
      }
      
      if (!IsThisPointer && !IsThisDeref) return;
      
      // Check if this cast is used in a member call expression
      if (auto *Parent = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("memberCall")) {
        //Get the source code text of target node
        std::string CastText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                       CE->getSourceRange());
        std::string CallText = stringutils::rangetoStr(*(Result.SourceManager),
                                                       Parent->getSourceRange());
        
        // Extract method name from the call
        if (auto *ME = Parent->getMethodDecl()) {
          std::string MethodName = ME->getNameAsString();
          
          //Perform mutation on the source code text by applying string replacement
          std::string Replacement;
          if (IsThisDeref) {
            // static_cast<Derived>(*this).method() -> this->method()
            Replacement = "this->" + MethodName;
          } else {
            // static_cast<Derived*>(this)->method() -> this->method()
            Replacement = "this->" + MethodName;
          }
          
          // Replace the cast expression and the dot/arrow operator with just this->
          size_t CastPos = CallText.find(CastText);
          if (CastPos != std::string::npos) {
            CallText.replace(CastPos, CastText.length(), Replacement);
            
            // Also need to handle the arguments
            std::string ArgsText;
            if (Parent->getNumArgs() > 0) {
              SourceRange ArgsRange(Parent->getArg(0)->getBeginLoc(),
                                   Parent->getArg(Parent->getNumArgs() - 1)->getEndLoc());
              ArgsText = stringutils::rangetoStr(*(Result.SourceManager), ArgsRange);
              CallText += "(" + ArgsText + ")";
            } else {
              CallText += "()";
            }
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Parent->getSourceRange()), 
                               "/*mut59*/" + CallText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxStaticCastExpr(
      hasAncestor(cxxMemberCallExpr().bind("memberCall"))
    ).bind("staticCastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}