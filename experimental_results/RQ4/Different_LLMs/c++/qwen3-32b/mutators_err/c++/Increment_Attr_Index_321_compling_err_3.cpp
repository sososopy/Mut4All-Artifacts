//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increment_Attr_Index_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Increment_Attr_Index_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Func = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!Func || !Result.Context->getSourceManager().isWrittenInMainFile(Func->getLocation()))
            return;
        if (!Func->isVariadic())
            return;
        for (const auto *A : Func->getAttrs()) {
            if (A->getKind() == clang::attr::Unknown) {
                if (A->getNumArgs() >= 1) {
                    const clang::Expr *Arg = A->getArg(0);
                    if (const auto *IntLit = dyn_cast<clang::IntegerLiteral>(Arg)) {
                        unsigned fixedParams = Func->getNumParams();
                        unsigned newIdx = fixedParams + 1;
                        std::string replacement = std::to_string(newIdx);
                        clang::SourceRange sr = IntLit->getSourceRange();
                        if (sr.isValid()) {
                            Rewrite.ReplaceText(sr, replacement);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isVariadic(), hasAttr(clang::attr::Unknown, clang::ast_matchers::attr().hasArgument(0, integerLiteral()))).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}