//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_pseudo_destructor_expression_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_pseudo_destructor_expression_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *E = Result.Nodes.getNodeAs<clang::CXXPseudoDestructorExpr>("PseudoDestructor")) {
        if (!E || !Result.Context->getSourceManager().isWrittenInMainFile(E->getBeginLoc()))
            return;

        auto &SM = Result.Context->getSourceManager();
        auto &LangOpts = Result.Context->getLangOpts();
        auto StartLoc = E->getBeginLoc();
        auto EndLoc = E->getEndLoc();

        if (StartLoc.isValid() && EndLoc.isValid()) {
            auto OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts);
            if (!OriginalText.empty()) {
                std::string MutatedText = OriginalText.str();
                size_t pos = MutatedText.find("~");
                if (pos != std::string::npos) {
                    pos = MutatedText.find('(', pos);
                    if (pos != std::string::npos) {
                        MutatedText.insert(pos, "...");
                    }
                }
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), MutatedText);
            }
        }
    }
}

void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxPseudoDestructorExpr().bind("PseudoDestructor");
    MutatorFrontendAction_16::Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}