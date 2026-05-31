//header file
#pragma once
#include "Mutator_base.h"

/**
 * template_function_pointer_dereference_625
 */ 
class MutatorFrontendAction_625 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(625)

private:
    class MutatorASTConsumer_625 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_625(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_function_pointer_dereference_625.h"

// ========================================================================================================
#define MUT625_OUTPUT 1

void MutatorFrontendAction_625::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TemplateFunction")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;

        if (MT->isTemplateInstantiation()) {
            auto body = MT->getBody();
            for (auto &stmt : body->children()) {
                if (auto *castExpr = llvm::dyn_cast<clang::CStyleCastExpr>(stmt)) {
                    auto castType = castExpr->getTypeAsWritten();
                    if (castType->isPointerType()) {
                        std::string replacement = "*(" + castExpr->getSubExprAsWritten()->getSourceRange().getBegin().printToString(Result.Context->getSourceManager()) + ") = 0;";
                        Rewrite.ReplaceText(castExpr->getSourceRange(), replacement);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_625::MutatorASTConsumer_625::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}