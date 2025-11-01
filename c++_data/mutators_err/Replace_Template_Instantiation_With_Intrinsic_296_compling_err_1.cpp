//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Instantiation_With_Intrinsic_296
 */ 
class MutatorFrontendAction_296 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(296)

private:
    class MutatorASTConsumer_296 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_296(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Instantiation_With_Intrinsic_296.h"

// ========================================================================================================
#define MUT296_OUTPUT 1

void MutatorFrontendAction_296::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("templateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTemplateNameLoc()))
        return;
      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), 
                                                      Result.Context->getSourceManager(), 
                                                      Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "__make_integer_seq<" + originalText + ", 1, 2>";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_296::MutatorASTConsumer_296::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = templateSpecializationType().bind("templateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}