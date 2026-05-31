//header file
#pragma once
#include "Mutator_base.h"

/**
 * Annotate_Macro_266
 */ 
class MutatorFrontendAction_266 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(266)

private:
    class MutatorASTConsumer_266 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_266(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Annotate_Macro_266.h"

// ========================================================================================================
#define MUT266_OUTPUT 1

void MutatorFrontendAction_266::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MacroDirective>("Macros")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto range = MT->getMacroInfo()->getDefinitionLoc();
      auto name_range = stringutils::rangetoStr(*(Result.SourceManager), range);
      auto name_loc = range.getLocWithOffset(name_range.size());
      //Perform mutation on the source code text by applying string replacement
      auto annotation = "/*mut266*/[[clang::annotate(\"opt\")]] ";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(name_loc, 0, annotation);
    }
}
  
void MutatorFrontendAction_266::MutatorASTConsumer_266::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::macroDirective().bind("Macros");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}