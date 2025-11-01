//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Enum_Declaration_268
 */ 
class MutatorFrontendAction_268 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(268)

private:
    class MutatorASTConsumer_268 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_268(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_268.h"

// ========================================================================================================
#define MUT268_OUTPUT 1

void MutatorFrontendAction_268::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                       ED->getLocation()))
            return;

        auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());
        std::string invalidEnum = "int"; // Example of invalid enumerator
        if (enumSource.find(invalidEnum) == std::string::npos) {
            size_t pos = enumSource.rfind('}');
            if (pos != std::string::npos) {
                enumSource.insert(pos, ", " + invalidEnum);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
            }
        }
    } else if (Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
        SourceLocation insertLoc = Result.Context->getTranslationUnitDecl()->getBeginLoc();
        std::string newEnum = "\nenum Status { Valid, Invalid, 123Invalid }; // Invalid enumerator\n";
        Rewrite.InsertText(insertLoc, newEnum, true, true);
    }
}

void MutatorFrontendAction_268::MutatorASTConsumer_268::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher enumMatcher = enumDecl().bind("EnumDecl");
    DeclarationMatcher translationUnitMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(translationUnitMatcher, &callback);
    matchFinder.matchAST(Context);
}