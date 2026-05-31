//header file
#pragma once
#include "Mutator_base.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * misordered_embed_directive_parameters_32
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
#include "../include/misordered_embed_directive_parameters_32.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
      if (!PPD || !Result.SourceManager->isWrittenInMainFile(PPD->getLocation()))
        return;

      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PPD->getSourceRange());
      size_t filePos = directiveText.find("__FILE__");
      if (filePos != std::string::npos) {
          size_t unknownParamPos = directiveText.find("unknownParam");
          if (unknownParamPos != std::string::npos && unknownParamPos < filePos) {
              std::swap(directiveText[filePos], directiveText[unknownParamPos]);
          }
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), directiveText);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = preprocessingDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}