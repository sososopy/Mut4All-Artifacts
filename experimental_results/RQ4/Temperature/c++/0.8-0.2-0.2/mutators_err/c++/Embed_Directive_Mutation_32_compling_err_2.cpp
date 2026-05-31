//header file
#pragma once
#include "Mutator_base.h"

/**
 * embed_directive_mutation_32
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
#include "../include/embed_directive_mutation_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PD = Result.Nodes.getNodeAs<clang::PreprocessingDirective>("EmbedDirective")) {
      //Filter nodes in header files
      if (!PD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PD->getLocation()))
        return;

      //Get the source code text of target node
      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = directiveText.find("__FILE__");
      if (pos != std::string::npos) {
        directiveText.insert(pos + 8, " unknownParam");
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(PD->getSourceRange()), directiveText);
      }
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = preprocessingDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}