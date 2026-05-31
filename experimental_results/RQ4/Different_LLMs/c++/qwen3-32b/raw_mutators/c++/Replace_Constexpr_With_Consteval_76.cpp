//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Consteval_76
 */ 
class MutatorFrontendAction_76 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(76)

private:
    class MutatorASTConsumer_76 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_76(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Constexpr_With_Consteval_76.h"

// ========================================================================================================
#define MUT76_OUTPUT 1

void MutatorFrontendAction_76::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Func")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Get the source code text of the target node
        auto text = stringutils::rangetoStr(*Result.SourceManager, FD->getSourceRange());
        size_t pos = text.find("constexpr");
        if (pos != std::string::npos) {
            text.replace(pos, 9, "consteval"); // 9 is length of "constexpr"
            Rewrite.ReplaceText(FD->getSourceRange(), text);
        }
    }
}

void MutatorFrontendAction_76::MutatorASTConsumer_76::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (functionDecl(isConstexpr()).bind("Func") || cxxMethodDecl(isConstexpr()).bind("Func"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}