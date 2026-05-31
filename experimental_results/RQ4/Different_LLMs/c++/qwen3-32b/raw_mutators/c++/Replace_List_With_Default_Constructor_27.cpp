//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_List_With_Default_Constructor_27
 */ 
class MutatorFrontendAction_27 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(27)

private:
    class MutatorASTConsumer_27 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_27(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };

};

//source file
#include "../include/Replace_List_With_Default_Constructor_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        // Filter header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        Expr *Init = VD->getInit();
        if (!Init)
            return;

        if (auto *ILE = dyn_cast<InitListExpr>(Init)) {
            if (ILE->getNumExprs() == 0) {
                SourceLocation StartLoc = ILE->getLBraceLoc();
                SourceLocation EndLoc = ILE->getRBraceLoc();
                if (StartLoc.isValid() && EndLoc.isValid()) {
                    Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), "()");
                }
            }
        }
    }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr(hasSize(0)))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}