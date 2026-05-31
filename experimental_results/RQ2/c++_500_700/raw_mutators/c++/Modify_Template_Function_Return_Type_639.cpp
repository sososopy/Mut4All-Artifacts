//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_function_return_type_639
 */ 
class MutatorFrontendAction_639 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(639)

private:
    class MutatorASTConsumer_639 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_639(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_function_return_type_639.h"

// ========================================================================================================
#define MUT639_OUTPUT 1

void MutatorFrontendAction_639::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RT = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                RT->getLocation()))
            return;

        if (const auto *Body = RT->getBody()) {
            for (const auto *Stmt : Body->children()) {
                if (const auto *ReturnStmt = dyn_cast<clang::ReturnStmt>(Stmt)) {
                    const auto *RetExpr = ReturnStmt->getRetValue();
                    if (RetExpr && isa<clang::CXXConstructExpr>(RetExpr)) {
                        auto ReturnRange = ReturnStmt->getSourceRange();
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ReturnRange), "return {};");
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_639::MutatorASTConsumer_639::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), returns(hasDescendant(templateTypeParmType()))).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}