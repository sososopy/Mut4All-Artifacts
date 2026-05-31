//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Module_Interface_And_Global_81
 */ 
class MutatorFrontendAction_81 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(81)
private:
    class MutatorASTConsumer_81 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_81(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Module_Interface_And_Global_81.h"

// ========================================================================================================
#define MUT81_OUTPUT 1

void MutatorFrontendAction_81::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        ASTContext *Context = Result.Context;
        bool hasModule = false;
        for (Decl *D : Context->getTranslationUnitDecl()->decls()) {
            if (isa<clang::ModuleDecl>(D)) {
                hasModule = true;
                break;
            }
        }

        if (!hasModule) {
            SourceManager &SM = Rewrite.getSourceMgr();
            FileID mainFileID = SM.getMainFileID();
            SourceLocation startLoc = SM.getLocForStartOfFile(mainFileID);
            if (startLoc.isValid()) {
                Rewrite.InsertText(startLoc, "module;\n", true, true);
            }
        }
    }
}
  
void MutatorFrontendAction_81::MutatorASTConsumer_81::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}