//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_Block_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)
private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_block_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            std::string unreachableCode = "\n/*mut284*/if (false) {\n    int unreachable = 0;\n}\n";

            if (bodyText.rfind('}') != std::string::npos)
                bodyText.insert(bodyText.rfind('}'), unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), bodyText);
        }
    }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}