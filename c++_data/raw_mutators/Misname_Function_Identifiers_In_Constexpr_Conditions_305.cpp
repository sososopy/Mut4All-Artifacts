//header file
#pragma once
#include "Mutator_base.h"

/**
 * Misname_Function_Identifiers_In_Constexpr_Conditions_305
 */ 
class MutatorFrontendAction_305 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(305)

private:
    class MutatorASTConsumer_305 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_305(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misname_function_identifiers_in_constexpr_conditions_305.h"

// ========================================================================================================
#define MUT305_OUTPUT 1

void MutatorFrontendAction_305::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ICE = Result.Nodes.getNodeAs<clang::IfStmt>("IfConstexpr")) {
      if (!ICE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ICE->getBeginLoc()))
        return;

      if (ICE->isConstexpr()) {
        auto condition = ICE->getCond();
        if (auto *callExpr = dyn_cast<CallExpr>(condition)) {
          if (auto *callee = callExpr->getDirectCallee()) {
            std::string originalName = callee->getNameInfo().getName().getAsString();
            if (originalName.empty()) return;

            std::string mutatedName = originalName;
            if (mutatedName.length() > 1) {
              std::swap(mutatedName[0], mutatedName[1]);
            }

            SourceLocation startLoc = callExpr->getBeginLoc();
            SourceLocation endLoc = Lexer::getLocForEndOfToken(callExpr->getEndLoc(), 0, *Result.SourceManager, Result.Context->getLangOpts());
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), mutatedName);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_305::MutatorASTConsumer_305::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    StatementMatcher matcher = ifStmt(isConstexpr()).bind("IfConstexpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}