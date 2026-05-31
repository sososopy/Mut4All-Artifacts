//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Enum_Forward_Bool_StaticAssert_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Change_Enum_Forward_Bool_StaticAssert_31.h"
#include "clang/Lex/Lexer.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        // Filter nodes in header files
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        std::string enumName = ED->getNameAsString();
        if (enumName.empty())
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation start = ED->getBeginLoc();
        SourceLocation end = ED->getEndLoc();

        if (SM.isInSystemHeader(start) || SM.isInSystemHeader(end))
            return;

        // Replace the enum declaration with forward declaration
        std::string replacement = "enum " + enumName + " : bool;";
        Rewrite.ReplaceText(SourceRange(start, end), replacement);

        // Insert static_assert after the semicolon
        SourceLocation insertLoc = Lexer::getEndOfToken(end, SM, Result.Context->getLangOpts());
        std::string saText = "static_assert(!std::is_function<" + enumName + ">::value, \"\");";
        Rewrite.InsertText(insertLoc, "\n" + saText, true, true);
    }
}

void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}