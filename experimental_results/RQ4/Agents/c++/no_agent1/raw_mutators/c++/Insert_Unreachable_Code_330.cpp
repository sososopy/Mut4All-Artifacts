//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unreachable_Code_330
 */ 

class MutatorFrontendAction_330 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(330)
private:
    class MutatorASTConsumer_330 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_330(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unreachable_code_330.h"

// ========================================================================================================
#define MUT330_OUTPUT 1

void MutatorFrontendAction_330::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getLocation()))
            return;

        if (FS->hasBody()) {
            auto body = FS->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            // Insert unreachable code at the end of the function body
            std::string unreachableCode = "\n/*mut330*/ if (false) { int unreachable = 0; unreachable++; }\n";
            if (bodyText.rfind('}') != std::string::npos) {
                bodyText.insert(bodyText.rfind('}'), unreachableCode);
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyText);
        }
    }
}

void MutatorFrontendAction_330::MutatorASTConsumer_330::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}