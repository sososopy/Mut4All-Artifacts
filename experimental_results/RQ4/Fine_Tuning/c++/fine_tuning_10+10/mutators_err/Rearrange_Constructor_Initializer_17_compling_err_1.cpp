//header file
#pragma once
#include "Mutator_base.h"

/**
 * Rearrange_Constructor_Initializer_17
 */ 
class MutatorFrontendAction_17 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(17)

private:
    class MutatorASTConsumer_17 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_17(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/rearrange_constructor_initializer_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctorDecl")) {
        if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(CtorDecl->getLocation()))
            return;

        if (CtorDecl->getNumCtorInitializers() < 2)
            return;

        std::vector<const clang::CXXCtorInitializer*> initializers;
        for (auto *init : CtorDecl->inits()) {
            initializers.push_back(init);
        }

        if (initializers.size() < 2)
            return;

        std::swap(initializers[0], initializers[1]);

        std::string newInitList;
        for (const auto *init : initializers) {
            if (!newInitList.empty()) {
                newInitList += ", ";
            }
            newInitList += clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(init->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();
        }

        clang::SourceLocation startLoc = CtorDecl->getInitLoc();
        clang::SourceLocation endLoc = CtorDecl->getEndLoc();

        Rewrite.ReplaceText(clang::SourceRange(startLoc, endLoc), newInitList);
    }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(isExplicit()).bind("ctorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}