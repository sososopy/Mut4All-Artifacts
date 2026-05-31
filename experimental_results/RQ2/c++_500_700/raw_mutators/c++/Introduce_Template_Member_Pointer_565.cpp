//header file
#pragma once
#include "Mutator_base.h"

/**
 * introduce_template_member_pointer_565
 */ 
class MutatorFrontendAction_565 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(565)

private:
    class MutatorASTConsumer_565 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_565(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_template_member_pointer_565.h"

// ========================================================================================================
#define MUT565_OUTPUT 1

void MutatorFrontendAction_565::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithMemberPointer")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionBody = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());

            std::string mutation = "\n/*mut565*/template <typename T> struct B { T A::*ptr; };\n";
            mutation += "B<int> b;\n";
            mutation += "a.*b.ptr = 10; // Improper use of member pointer with template\n";

            functionBody.insert(functionBody.find("{") + 1, mutation);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
        }
    }
}

void MutatorFrontendAction_565::MutatorASTConsumer_565::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(memberExpr())).bind("FunctionWithMemberPointer");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}