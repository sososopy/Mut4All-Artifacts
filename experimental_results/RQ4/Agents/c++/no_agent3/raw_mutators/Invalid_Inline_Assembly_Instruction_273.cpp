//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Inline_Assembly_Instruction_273
 */ 
class MutatorFrontendAction_273 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(273)

private:
    class MutatorASTConsumer_273 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_273(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/invalid_inline_assembly_instruction_273.h"

// ========================================================================================================
#define MUT273_OUTPUT 1

void MutatorFrontendAction_273::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            bool hasAsm = false;

            for (auto &stmt : body->children()) {
                if (isa<clang::GCCAsmStmt>(stmt)) {
                    hasAsm = true;
                    break;
                }
            }

            if (!hasAsm) {
                auto firstVar = body->child_begin();
                if (firstVar != body->child_end()) {
                    if (auto *DS = dyn_cast<clang::DeclStmt>(*firstVar)) {
                        if (DS->isSingleDecl()) {
                            if (auto *VD = dyn_cast<clang::VarDecl>(DS->getSingleDecl())) {
                                std::string asmCode = "__asm__(\"invalid_instruction %0\" : : \"r\"(" + VD->getNameAsString() + "));";
                                Rewrite.InsertTextAfterToken(body->getBeginLoc(), "\n" + asmCode);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_273::MutatorASTConsumer_273::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}