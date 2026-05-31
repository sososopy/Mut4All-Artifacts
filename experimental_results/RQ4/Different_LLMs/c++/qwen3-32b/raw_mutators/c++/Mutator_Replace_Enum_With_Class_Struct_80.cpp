//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Replace_Enum_With_Class_Struct_80
 */ 
class MutatorFrontendAction_80 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(80)

private:
    class MutatorASTConsumer_80 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_80(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Enum_With_Class_Struct_80.h"

// ========================================================================================================
#define MUT80_OUTPUT 1

void MutatorFrontendAction_80::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UED = Result.Nodes.getNodeAs<clang::UsingEnumDecl>("usingEnumDecl")) {
        if (!UED || !Result.Context->getSourceManager().isWrittenInMainFile(UED->getLocation()))
            return;

        SourceLocation enumNameLoc = UED->getEnumNameLoc();
        if (enumNameLoc.isInvalid())
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        if (!SM.isWrittenInMainFile(enumNameLoc))
            return;

        std::string replacement = (getrandom::getRandomIndex(1) == 0) ? "class" : "struct";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(enumNameLoc), replacement);
    }
}

void MutatorFrontendAction_80::MutatorASTConsumer_80::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = usingEnumDecl().bind("usingEnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}