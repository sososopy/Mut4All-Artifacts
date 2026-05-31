//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Move_Capture_With_Value_Capture_36
 */ 
class MutatorFrontendAction_Replace_Move_Capture_With_Value_Capture_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Move_Capture_With_Value_Capture_36)
private:
    class MutatorASTConsumer_Replace_Move_Capture_With_Value_Capture_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Move_Capture_With_Value_Capture_36(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Move_Capture_With_Value_Capture_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_Replace_Move_Capture_With_Value_Capture_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DS = Result.Nodes.getNodeAs<clang::DeclStmt>("lambdaStmt")) {
        if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(DS->getBeginLoc()))
            return;

        for (auto *D : DS->decls()) {
            if (auto *VD = dyn_cast<clang::VarDecl>(D)) {
                if (VD->hasInit() && isa<clang::LambdaExpr>(VD->getInit())) {
                    auto *LE = cast<clang::LambdaExpr>(VD->getInit());
                    for (unsigned i = 0; i < LE->capture_size(); ++i) {
                        const auto *Cap = LE->capture_begin()[i];
                        if (Cap->getCaptureKind() == clang::LCK_InitCapture) {
                            const auto *Init = Cap->getInit();
                            if (auto *CE = dyn_cast<clang::CallExpr>(Init)) {
                                const auto *FD = CE->getCalleeDecl();
                                if (FD && FD->getQualifiedNameAsString() == "std::move") {
                                    const auto *Arg = CE->getArg(0);
                                    SourceLocation Start = Arg->getBeginLoc();
                                    SourceLocation End = Arg->getEndLoc();
                                    std::string ArgText = clang::Lexer::getSourceText(
                                        clang::CharSourceRange::getTokenRange(Start, End),
                                        *Result.SourceManager,
                                        Result.Context->getLangOpts()
                                    ).str();
                                    Rewrite.ReplaceText(CE->getSourceRange(), ArgText);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_Replace_Move_Capture_With_Value_Capture_36::MutatorASTConsumer_Replace_Move_Capture_With_Value_Capture_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declStmt(
        hasInitializer(
            lambdaExpr(
                hasAnyCapture(
                    lambdaCapture(
                        hasInitializer(
                            callExpr(
                                callee(
                                    functionDecl(
                                        hasName("std::move")
                                    )
                                )
                            )
                        )
                    )
                )
            )
        )
    ).bind("lambdaStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}