//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Concept_Constraints_In_Template_Parameter_88
 */ 
class MutatorFrontendAction_88 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(88)

private:
    class MutatorASTConsumer_88 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_88(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Concept_Constraints_In_Template_Parameter_88.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT88_OUTPUT 1

void MutatorFrontendAction_88::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TPL = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TPL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TPL->getLocation()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Rewrite.getLangOpts();
        auto StartLoc = TPL->getBeginLoc();
        auto EndLoc = TPL->getEndLoc();

        if (StartLoc.isInvalid() || EndLoc.isInvalid())
            return;

        std::string OriginalText = Lexer::getSourceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), SM, LangOpts).str();
        
        size_t requiresPos = OriginalText.find("requires");
        if (requiresPos != std::string::npos) {
            size_t endPos = OriginalText.find("{", requiresPos);
            if (endPos != std::string::npos) {
                std::string newConstraint = "requires (MyConcept<T> && AnotherConcept<T>)";
                OriginalText.replace(requiresPos, endPos - requiresPos, newConstraint);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc), OriginalText);
            }
        }
    }
}

void MutatorFrontendAction_88::MutatorASTConsumer_88::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}