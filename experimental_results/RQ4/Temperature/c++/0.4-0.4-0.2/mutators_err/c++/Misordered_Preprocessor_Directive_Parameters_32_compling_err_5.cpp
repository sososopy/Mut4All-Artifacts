//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"

/**
 * misordered_preprocessor_directive_parameters_32
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
#include "../include/misordered_preprocessor_directive_parameters_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessorDirective>("PreprocessorDirective")) {
        if (!PPD || !Result.SourceManager->isWrittenInMainFile(PPD->getSourceRange().getBegin()))
            return;

        auto directiveText = stringutils::rangetoStr(*(Result.SourceManager), PPD->getSourceRange());

        size_t limitPos = directiveText.find("limit(");
        if (limitPos != std::string::npos) {
            size_t endPos = directiveText.find(")", limitPos);
            if (endPos != std::string::npos) {
                std::string unknownParam = "unknown_param(2) ";
                directiveText.insert(limitPos, unknownParam);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), directiveText);
            }
        }
    }
}

void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    using namespace clang::ast_matchers;
    auto matcher = preprocessingDirective().bind("PreprocessorDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}