//header file
#pragma once
#include "Mutator_base.h"

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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/misordered_embed_directive_parameters_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PPD = Result.Nodes.getNodeAs<clang::PreprocessorDirective>("EmbedDirective")) {
      //Filter nodes in header files
      if (!PPD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PPD->getLocation()))
        return;

      //Get the source code text of target node
      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PPD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t filePos = directiveText.find("__FILE__");
      if (filePos != std::string::npos) {
          size_t unknownParamPos = directiveText.find("unknownParam");
          if (unknownParamPos != std::string::npos && unknownParamPos < filePos) {
              std::swap(directiveText[filePos], directiveText[unknownParamPos]);
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PPD->getSourceRange()), directiveText);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = preprocessorDirective().bind("EmbedDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}