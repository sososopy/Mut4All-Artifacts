//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Consteval_Constructor_Invocation_324
 */ 
class MutatorFrontendAction_324 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(324)

private:
    class MutatorASTConsumer_324 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_324(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Consteval_Constructor_Invocation_324.h"

// ========================================================================================================
#define MUT324_OUTPUT 1

void MutatorFrontendAction_324::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constevalCtor")) {
        if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(Ctor->getLocation()))
            return;

        if (Ctor->isConsteval()) {
            auto body = Ctor->getBody();
            if (body) {
                auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
                std::string additionalFuncDecl = "extern constexpr int additionalFunc() { return 42; }\n";
                std::string additionalCall = "additionalFunc();";
                if (bodyStr.find(additionalCall) == std::string::npos) {
                    auto insertPos = bodyStr.find_last_of('}');
                    if (insertPos != std::string::npos) {
                        bodyStr.insert(insertPos, additionalCall);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
                        Rewrite.InsertTextBefore(Ctor->getBeginLoc(), additionalFuncDecl);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_324::MutatorASTConsumer_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(isConsteval()).bind("constevalCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}