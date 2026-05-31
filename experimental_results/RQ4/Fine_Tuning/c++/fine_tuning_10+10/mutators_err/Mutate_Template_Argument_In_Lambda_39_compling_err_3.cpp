//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Template_Argument_In_Lambda_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)

private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXMethodDecl *TargetLambda = nullptr;
    };
};

//source file
#include "../include/Mutate_Template_Argument_In_Lambda_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("LambdaCall")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (TargetLambda == nullptr) {
        TargetLambda = MT;
      }

      if (MT == TargetLambda) {
        auto *body = MT->getBody();
        if (!body) return;

        for (auto it = body->child_begin(); it != body->child_end(); ++it) {
          if (auto *callExpr = dyn_cast<clang::CallExpr>(*it)) {
            std::string originalCall = stringutils::rangetoStr(*(Result.SourceManager), callExpr->getSourceRange());

            std::string mutatedCall = originalCall;
            size_t pos = mutatedCall.find("<");
            if (pos != std::string::npos) {
              mutatedCall.insert(pos + 1, "(f + 1), ");
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(callExpr->getSourceRange()), mutatedCall);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isLambda()).bind("LambdaCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}