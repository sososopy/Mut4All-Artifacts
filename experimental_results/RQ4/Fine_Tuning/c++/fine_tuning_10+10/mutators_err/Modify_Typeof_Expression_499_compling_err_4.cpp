//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Typeof_Expression_499
 */ 
class MutatorFrontendAction_499 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(499)

private:
    class MutatorASTConsumer_499 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_499(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Typeof_Expression_499.h"

// ========================================================================================================
#define MUT499_OUTPUT 1

void MutatorFrontendAction_499::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeOfExpr>("TypeOfExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTypeOfLoc()))
        return;
      
      auto originalExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      std::string mutatedExpr = originalExpr;

      size_t pos = mutatedExpr.find("sizeof(c) <= 1");
      if (pos != std::string::npos) {
          mutatedExpr.replace(pos, std::string("sizeof(c) <= 1").length(), "(sizeof(c) & 1) > 0");
          mutatedExpr = "/*mut499*/" + mutatedExpr;
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedExpr);
      }
    }
}
  
void MutatorFrontendAction_499::MutatorASTConsumer_499::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = typeOfExpr(hasDescendant(binaryOperator(
                        hasOperatorName("<="), 
                        hasLHS(ignoringParenImpCasts(unaryExprOrTypeTraitExpr(hasDescendant(declRefExpr().bind("c"))))),
                        hasRHS(integerLiteral(equals(1)))
                    ))).bind("TypeOfExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}