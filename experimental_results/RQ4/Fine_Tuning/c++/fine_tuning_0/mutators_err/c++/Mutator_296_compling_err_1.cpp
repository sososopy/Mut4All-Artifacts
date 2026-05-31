//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_296
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
#include "../include/Mutator_296.h"

// ========================================================================================================
#define MUT296_OUTPUT 1

void MutatorFrontendAction_296::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnresolvedLookupExpr>("MakeIntegerSeq")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getName().getAsString();
      if (name != "__make_integer_seq")
        return;
      auto args = MT->template_arguments();
      if (args.empty())
        return;
      auto first_arg = args[0];
      auto first_arg_source = stringutils::rangetoStr(
          *(Result.SourceManager), first_arg.getSourceRange());
      llvm::outs() << first_arg_source << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto new_first_arg = "int";
      auto new_args_source = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getSourceRange());
      new_args_source.replace(new_args_source.find(first_arg_source),
                              first_arg_source.length(), new_first_arg);
      llvm::outs() << new_args_source << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), new_args_source);
    }
}
  
void MutatorFrontendAction_296::MutatorASTConsumer_296::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unresolvedLookupExpr().bind("MakeIntegerSeq");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}