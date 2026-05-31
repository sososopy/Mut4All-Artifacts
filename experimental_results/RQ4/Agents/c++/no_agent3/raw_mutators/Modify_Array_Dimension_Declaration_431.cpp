//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Dimension_Declaration_431
 */ 
class MutatorFrontendAction_431 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(431)

private:
    class MutatorASTConsumer_431 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_431(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Array_Dimension_Declaration_431.h"

// ========================================================================================================
#define MUT431_OUTPUT 1

void MutatorFrontendAction_431::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *AT = VD->getType()->getAsArrayTypeUnsafe()) {
        if (auto *CAT = dyn_cast<ConstantArrayType>(AT)) {
          auto sizeExpr = CAT->getSizeExpr();
          if (sizeExpr) {
            std::string newExpr = (llvm::Twine(sizeExpr->getValue().getZExtValue()) + " + 5 * 0").str();
            Rewrite.ReplaceText(sizeExpr->getSourceRange(), newExpr);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_431::MutatorASTConsumer_431::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("arrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}