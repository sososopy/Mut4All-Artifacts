//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Prvlue_Assignment_Constexpr_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(69)

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Prvlue_Assignment_Constexpr_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("expr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        QualType T = CE->getType();
        CXXRecordDecl *RD = T->getAsCXXRecordDecl();
        if (!RD || !RD->isCompleteDefinition())
            return;

        bool hasDeletedCopy = false;
        for (auto *Ctor : RD->ctors()) {
            if (Ctor->isCopyConstructor() && Ctor->isDeleted()) {
                hasDeletedCopy = true;
                break;
            }
        }
        if (!hasDeletedCopy)
            return;

        bool hasConsteval = false;
        for (auto *Method : RD->methods()) {
            if (Method->isConstexpr()) {
                hasConsteval = true;
                break;
            }
        }
        if (!hasConsteval)
            return;

        const FunctionDecl *FD = Result.Context->getFunctionDecl(CE);
        if (!FD || !FD->isConstexpr())
            return;

        std::string typeName = RD->getNameAsString();
        std::string newExpr = "(" + typeName + "{} = " + typeName + "{})";
        newExpr = "/*mut69*/" + newExpr;

        SourceLocation startLoc = CE->getBeginLoc();
        SourceLocation endLoc = CE->getEndLoc();
        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newExpr);
    }
}
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructExpr().bind("expr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}