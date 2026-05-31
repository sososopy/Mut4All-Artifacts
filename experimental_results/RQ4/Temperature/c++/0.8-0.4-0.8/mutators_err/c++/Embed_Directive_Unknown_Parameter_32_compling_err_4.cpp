//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceManager.h"

/**
 * embed_directive_unknown_parameter_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/embed_directive_unknown_parameter_32.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
        if (!PPD || !Result.SourceManager->isWrittenInMainFile(PPD->getSourceRange().getBegin()))
            return;

        auto directiveText = stringutils::rangetoStr(*(Result.SourceManager), PPD->getSourceRange());
        size_t knownParamPos = directiveText.find("__FILE__");
        if (knownParamPos != std::string::npos) {
            directiveText.insert(knownParamPos + std::string("__FILE__").length(), " unknownparam(3)");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), directiveText);
        }
    }
}

void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = clang::ast_matchers::ppIncludeDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}