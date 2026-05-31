//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Assert_Replacement_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Static_Assert_Replacement_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        Expr *Init = VD->getInit();
        if (!Init)
            return;

        QualType VarType = VD->getType();
        const Type *Ty = VarType.getTypePtr();
        if (!Ty)
            return;

        SourceLocation InitStart = Init->getBeginLoc();
        SourceLocation InitEnd = Init->getEndLoc();
        std::string InitText = stringutils::rangetoStr(*Result.SourceManager, SourceRange(InitStart, InitEnd));

        std::string Condition;
        if (Ty->isIntegerType()) {
            Condition = InitText + " > 0";
        } else if (Ty->isPointerType()) {
            Condition = InitText + " != nullptr";
        } else if (Ty->isBooleanType()) {
            Condition = InitText + " == true";
        } else {
            Condition = InitText;
        }

        std::string StaticAssert = "static_assert(" + Condition + ", \"\");";
        SourceRange SR = VD->getSourceRange();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(SR), StaticAssert);
    }
}
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(expr())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}