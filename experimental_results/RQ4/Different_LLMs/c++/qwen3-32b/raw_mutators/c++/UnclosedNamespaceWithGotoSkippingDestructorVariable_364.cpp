//header file
#pragma once
#include "Mutator_base.h"

/**
 * UnclosedNamespaceWithGotoSkippingDestructorVariable_364
 */ 
class MutatorFrontendAction_364 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(364)

private:
    class MutatorASTConsumer_364 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_364(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/UnclosedNamespaceWithGotoSkippingDestructorVariable_364.h"

// ========================================================================================================
#define MUT364_OUTPUT 1

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        SourceManager &SM = *Result.SourceManager;
        SourceLocation Start = FD->getBeginLoc();
        SourceLocation End = FD->getEndLoc();

        if (Start.isInvalid() || End.isInvalid())
            return;

        std::string funcCode = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(FD->getSourceRange()));

        size_t openBracePos = funcCode.find("{");
        if (openBracePos == std::string::npos)
            return;

        std::string beforeBrace = funcCode.substr(0, openBracePos + 1);
        std::string afterBrace = funcCode.substr(openBracePos + 1);

        std::string newLines = "\n    goto EXIT;\n    Dummy d;\nEXIT:";
        std::string modifiedBody = beforeBrace + newLines + afterBrace;

        std::string newCode = "namespace {\nstruct Dummy { ~Dummy(); };\n" + modifiedBody + "\n";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newCode);
    }
}

void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasBody(compoundStmt())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}