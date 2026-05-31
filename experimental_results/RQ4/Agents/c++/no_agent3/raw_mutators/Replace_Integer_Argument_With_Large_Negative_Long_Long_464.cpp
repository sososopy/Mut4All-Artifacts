//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Integer_Argument_With_Large_Negative_Long_Long_464
 */ 
class MutatorFrontendAction_464 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(464)

private:
    class MutatorASTConsumer_464 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_464(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Integer_Argument_With_Large_Negative_Long_Long_464.h"

// ========================================================================================================
#define MUT464_OUTPUT 1

void MutatorFrontendAction_464::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CE->getBeginLoc()))
            return;

        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            if (auto *IL = llvm::dyn_cast<clang::IntegerLiteral>(CE->getArg(i))) {
                llvm::APInt value = IL->getValue();
                if (value.isSignedIntN(32)) {
                    std::string replacement = "-9223372036854775808LL";
                    Rewrite.ReplaceText(IL->getSourceRange(), replacement);
                    break;
                }
            }
        }
    }
}

void MutatorFrontendAction_464::MutatorASTConsumer_464::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(hasAnyArgument(integerLiteral())).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}