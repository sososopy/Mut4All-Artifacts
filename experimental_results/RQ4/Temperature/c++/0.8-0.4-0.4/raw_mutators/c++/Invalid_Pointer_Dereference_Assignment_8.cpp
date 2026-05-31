//header file
#pragma once
#include "Mutator_base.h"

/**
 * invalid_pointer_dereference_assignment_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/invalid_pointer_dereference_assignment_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->hasInit()) {
        auto *InitExpr = VD->getInit();
        if (isa<IntegerLiteral>(InitExpr)) {
          auto InitRange = InitExpr->getSourceRange();
          std::string MutatedInit = "*" + InitExpr->getSourceRange().getBegin().printToString(Result.Context->getSourceManager());
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitRange), MutatedInit);
        }
      }
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(integerLiteral())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}