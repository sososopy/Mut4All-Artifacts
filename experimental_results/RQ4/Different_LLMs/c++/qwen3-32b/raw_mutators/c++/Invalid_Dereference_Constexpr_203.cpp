//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Dereference_Constexpr_203
 */ 
class MutatorFrontendAction_203 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(203)

private:
    class MutatorASTConsumer_203 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_203(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Dereference_Constexpr_203.h"

// ========================================================================================================
#define MUT203_OUTPUT 1

void MutatorFrontendAction_203::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        if (!VD->hasInit()) return;
        auto init = VD->getInit();
        if (!init) return;
        SourceLocation start = init->getBeginLoc();
        SourceLocation end = init->getEndLoc();
        if (start.isInvalid() || end.isInvalid()) return;
        // Generate a random integer between 0-9 for the dereference operand
        int operand = getrandom::getRandomInt(0, 9);
        std::string replacement = "*" + std::to_string(operand);
        Rewrite.ReplaceText(SourceRange(start, end), replacement);
    }
}

void MutatorFrontendAction_203::MutatorASTConsumer_203::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Match const or constexpr variables with initializers
    DeclarationMatcher matcher = varDecl(
        anyOf(
            hasType(isConstQualified()),
            hasAttr<ConstexprAttr>()
        ),
        hasInitializer(expr())
    ).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}