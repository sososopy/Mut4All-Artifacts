//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Return_Type_With_Template_Alias_177
 */ 
class MutatorFrontendAction_177 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(177)

private:
    class MutatorASTConsumer_177 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_177(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_Return_Type_With_Template_Alias_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LD = Result.Nodes.getNodeAs<clang::VarDecl>("lambdaVar")) {
        if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(LD->getLocation()))
            return;

        // Insert constexpr before the lambda
        SourceLocation insertLoc = LD->getLocation();
        Rewrite.InsertText(insertLoc, "constexpr int T = 0;\n", true, true);

        // Get the lambda expression
        auto *LE = dyn_cast<clang::LambdaExpr>(LD->getInit());
        if (!LE)
            return;

        // Get the return type's source range
        clang::TypeLoc returnTypeLoc = LE->getTrailingReturnType();
        if (!returnTypeLoc)
            return;

        SourceRange returnTypeRange = returnTypeLoc.getSourceRange();
        if (returnTypeRange.isInvalid())
            return;

        // Replace the return type with type<T>
        Rewrite.ReplaceText(returnTypeRange, "type<T>");
    }
}

void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(lambdaExpr(hasTrailingReturnType(true), hasReturnType(qualType(isVoid()))))).bind("lambdaVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}