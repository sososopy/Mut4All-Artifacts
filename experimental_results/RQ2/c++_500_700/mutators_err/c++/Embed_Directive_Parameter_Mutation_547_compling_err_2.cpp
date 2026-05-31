//header file
#pragma once
#include "Mutator_base.h"

/**
 * embed_directive_parameter_mutation_547
 */ 
class MutatorFrontendAction_547 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(547)

private:
    class MutatorASTConsumer_547 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_547(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/embed_directive_parameter_mutation_547.h"

// ========================================================================================================
#define MUT547_OUTPUT 1

void MutatorFrontendAction_547::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
      if (!PPD || !Result.Context->getSourceManager().isWrittenInMainFile(PPD->getLocation()))
        return;

      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager), PPD->getSourceRange());

      size_t knownPos = directiveText.find("known_param");
      size_t unknownPos = directiveText.find("unknown_param");

      if (knownPos != std::string::npos && unknownPos != std::string::npos && knownPos < unknownPos) {
        std::swap(knownPos, unknownPos);
        std::string mutatedText = directiveText.substr(0, unknownPos) +
                                  directiveText.substr(knownPos, directiveText.find(' ', knownPos) - knownPos) + " " +
                                  directiveText.substr(unknownPos, directiveText.find(' ', unknownPos) - unknownPos) +
                                  directiveText.substr(directiveText.find(' ', knownPos));
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_547::MutatorASTConsumer_547::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = preprocessingDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}