//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Initializer_With_NonConstVar_299
 */ 
class MutatorFrontendAction_299 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(299)
private:
    class MutatorASTConsumer_299 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_299(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> nonConstVars;
    };
};

//source file
#include "../include/Replace_Static_Initializer_With_NonConstVar_299.h"

// ========================================================================================================
#define MUT299_OUTPUT 1

void MutatorFrontendAction_299::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *varDecl = Result.Nodes.getNodeAs<clang::VarDecl>("nonConstVar")) {
        if (!varDecl || !Result.Context->getSourceManager().isWrittenInMainFile(varDecl->getLocation()))
            return;
        if (!varDecl->getType()->isConstQualified() && !varDecl->isConstexpr()) {
            nonConstVars.push_back(varDecl);
        }
    } 
    else if (auto *targetVar = Result.Nodes.getNodeAs<clang::VarDecl>("targetVar")) {
        if (!targetVar || !Result.Context->getSourceManager().isWrittenInMainFile(targetVar->getLocation()))
            return;
        if ((targetVar->hasStaticStorageDuration() || targetVar->isConstexpr()) && targetVar->hasInit()) {
            if (!nonConstVars.empty()) {
                const auto *targetCtx = targetVar->getDeclContext();
                for (const auto *nonConstVar : nonConstVars) {
                    const auto *varCtx = nonConstVar->getDeclContext();
                    DeclContext *current = targetCtx;
                    bool found = false;
                    while (current) {
                        if (current == varCtx) {
                            found = true;
                            break;
                        }
                        current = current->getParent();
                    }
                    if (found) {
                        std::string varName = nonConstVar->getNameAsString();
                        SourceRange initRange = targetVar->getInit()->getSourceRange();
                        Rewrite.ReplaceText(initRange, varName);
                        break;
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_299::MutatorASTConsumer_299::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher nonConstVarMatcher = varDecl(
        unless(isConst()),
        unless(isConstexpr())
    ).bind("nonConstVar");
    DeclarationMatcher targetVarMatcher = varDecl(
        anyOf(
            hasStorageClass(SC_Static),
            isConstexpr()
        ),
        hasInit()
    ).bind("targetVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nonConstVarMatcher, &callback);
    matchFinder.addMatcher(targetVarMatcher, &callback);
    matchFinder.matchAST(Context);
}