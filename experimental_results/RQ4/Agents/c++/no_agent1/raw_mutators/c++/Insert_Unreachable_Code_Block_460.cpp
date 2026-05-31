//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto body = FD->getBody();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

        std::string unreachableCode = "\n/*mut460*/if (false) { while(true) {}; }\n";
        bodyText.insert(bodyText.find("{") + 1, unreachableCode);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
      }
    }
}
  
void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}