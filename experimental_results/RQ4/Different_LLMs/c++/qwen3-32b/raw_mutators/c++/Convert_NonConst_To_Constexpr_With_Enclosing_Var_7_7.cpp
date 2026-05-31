//header file
#pragma once
#include "Mutator_base.h"

/**
 * Convert_NonConst_To_Constexpr_With_Enclosing_Var_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Convert_NonConst_To_Constexpr_With_Enclosing_Var_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("method")) {
        if (!method || !Result.Context->getSourceManager().isWrittenInMainFile(method->getLocation()))
            return;

        const DeclContext *ctx = method->getLexicalDeclContext();
        bool foundX = false;
        while (ctx) {
            if (const auto *FD = dyn_cast<clang::FunctionDecl>(ctx)) {
                for (const auto *param : FD->parameters()) {
                    if (param->getName() == "x") {
                        foundX = true;
                        break;
                    }
                }
                if (!foundX) {
                    for (const auto *decl : FD->decls()) {
                        if (const auto *VD = dyn_cast<clang::VarDecl>(decl)) {
                            if (VD->getName() == "x") {
                                foundX = true;
                                break;
                            }
                        }
                    }
                }
                if (foundX) break;
            }
            ctx = ctx->getParent();
        }
        if (!foundX) return;

        SourceRange returnTypeRange = method->getReturnTypeSourceRange();
        if (returnTypeRange.isValid()) {
            Rewrite.InsertText(returnTypeRange.getBegin(), "constexpr ", true, true);
        }
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        unless(isConst()),
        hasBody(stmt())
    ).bind("method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}