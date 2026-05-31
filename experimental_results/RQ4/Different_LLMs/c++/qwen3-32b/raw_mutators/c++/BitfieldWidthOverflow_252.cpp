//header file
#pragma once
#include "Mutator_base.h"

/**
 * BitfieldWidthOverflow_252
 */ 
class MutatorFrontendAction_252 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(252)

private:
    class MutatorASTConsumer_252 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_252(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/BitfieldWidthOverflow_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("bitField")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->getName().empty() || !FD->getBitWidth())
            return;
        QualType T = FD->getType();
        std::string typeName = T.getAsString();
        ASTContext &Context = *Result.Context;
        unsigned maxBits = Context.getTypeSize(T);
        std::string newExpr = "(" + typeName + ")-1 + 1";
        if (clang::Expr *bitWidth = FD->getBitWidth()) {
            SourceLocation start = bitWidth->getBeginLoc();
            SourceLocation end = bitWidth->getEndLoc();
            Rewrite.ReplaceText(SourceRange(start, end), newExpr);
        }
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(
        hasName(""),
        ofClass,
        hasBitWidth(expr().bind("bitWidthExpr"))
    ).bind("bitField");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}