//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Ptr_Call_With_Direct_163
 */ 
class MutatorFrontendAction_163 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(163)

private:
    class MutatorASTConsumer_163 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_163(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Member_Ptr_Call_With_Direct_163.h"

// ========================================================================================================
#define MUT163_OUTPUT 1

void MutatorFrontendAction_163::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *call = Result.Nodes.getNodeAs<clang::CallExpr>("call")) {
        // Filter nodes in header files
        if (!call || !Result.Context->getSourceManager().isWrittenInMainFile(
                       call->getBeginLoc()))
            return;

        // Get the callee as a DeclRefExpr
        auto *callee = call->getCallee();
        auto *declRef = dyn_cast<clang::DeclRefExpr>(callee);
        if (!declRef)
            return;

        // Check if the variable is a member function pointer
        auto *varDecl = dyn_cast<clang::VarDecl>(declRef->getDecl());
        if (!varDecl || !varDecl->getType()->isFunctionPointerType())
            return;

        // Get the source range of the callee (obj.*ptr)
        auto callRange = callee->getSourceRange();

        // Get the variable name (ptr)
        std::string varName = declRef->getName().str();

        // Replace the obj.*ptr with varName
        Rewrite.ReplaceText(callRange, varName);
    }
}

void MutatorFrontendAction_163::MutatorASTConsumer_163::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = callExpr(callee(declRefExpr(to(varDecl(hasType(memberPointerType())))))).bind("call");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}