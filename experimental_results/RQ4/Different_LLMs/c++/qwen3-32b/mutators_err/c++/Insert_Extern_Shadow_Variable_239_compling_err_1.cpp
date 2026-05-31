//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Extern_Shadow_Variable_239
 */ 
class MutatorFrontendAction_239 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(239)

private:
    class MutatorASTConsumer_239 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_239(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Extern_Shadow_Variable_239.h"

// ========================================================================================================
#define MUT239_OUTPUT 1

void MutatorFrontendAction_239::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        SmallVector<clang::VarDecl*, 4> vars;
        for (const auto *Param : FD->parameters()) {
            vars.push_back(cast<clang::VarDecl>(Param));
        }

        if (FD->getBody()) {
            for (const auto *Stmt : FD->getBody()->children()) {
                if (auto *VD = dyn_cast<clang::VarDecl>(Stmt)) {
                    vars.push_back(VD);
                }
            }
        }

        if (vars.empty())
            return;

        clang::VarDecl *VD = vars[0];
        clang::QualType type = VD->getType();
        std::string typeName = type.getAsString();
        std::string varName = VD->getNameAsString();

        std::string externDecl = "extern " + typeName + " " + varName + ";";

        SourceLocation bodyStart = FD->getBody()->getBeginLoc();
        SourceLocation insertLoc = Rewrite.getLocForEndOfToken(bodyStart);
        Rewrite.InsertText(insertLoc, externDecl + "\n", true, true);
    }
}

void MutatorFrontendAction_239::MutatorASTConsumer_239::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}