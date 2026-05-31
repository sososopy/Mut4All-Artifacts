//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Cast_In_Consteval_294
 */ 
class MutatorFrontendAction_294 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(294)

private:
    class MutatorASTConsumer_294 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_294(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_294.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXStaticCastExpr>("StaticCastExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Check if we are in a consteval context
      const FunctionDecl* FD = Result.Context->getEnclosingFunction(MT->getBeginLoc());
      if (!FD || !FD->isConsteval())
        return;
      //Check the cast destination type is a class type
      QualType DestType = MT->getTypeAsWritten();
      if (!DestType->isRecordType())
        //Check the source expression is *this
      const Expr* SubExpr = MT->getSubExpr();
      if (!SubExpr)
        return;
      //Look for UnaryOperator representing *this
      if (auto* UO = dyn_cast<UnaryOperator>(SubExpr)) {
        if (UO->getOpcode() != UO_Deref)
          return;
        Expr* UOSubExpr = UO->getSubExpr();
        if (!UOSubExpr)
          return;
        //Check if the subexpression is a CXXThisExpr
        if (auto* TE = dyn_cast<CXXThisExpr>(UOSubExpr)) {
          //Get the source code text of target node
          SourceRange CastRange = MT->getSourceRange();
          std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CastRange);
          //Perform mutation on the source code text by applying string replacement
          //Replace static_cast<Derived>(*this) with static_cast<Derived*>(this)
          //First find the destination type name
          std::string DestTypeName = DestType.getAsString();
          //Remove the *this part and replace with this
          size_t DerefPos = OriginalText.find("*this");
          if (DerefPos == std::string::npos)
            return;
          std::string MutatedText = OriginalText.substr(0, DerefPos) + "this" + OriginalText.substr(DerefPos + 5);
          //Now change Derived to Derived*
          size_t TypeEnd = MutatedText.find(">(");
          if (TypeEnd == std::string::npos)
            return;
          std::string FinalText = MutatedText.substr(0, TypeEnd) + "*" + MutatedText.substr(TypeEnd);
          //Also need to adjust any member access after the cast
          //Look for MemberExpr following the static_cast
          if (auto* Parent = dyn_cast<MemberExpr>(MT->getParentIgnoreImplicit())) {
            //Change dot to arrow
            SourceLocation DotLoc = Parent->getOperatorLoc();
            if (DotLoc.isValid()) {
              Rewrite.ReplaceText(DotLoc, 1, "->");
            }
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(CastRange), FinalText);
        }
      }
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxStaticCastExpr().bind("StaticCastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}