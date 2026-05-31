//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Constraint_With_False_479
 */ 
class MutatorFrontendAction_479 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(479)

private:
    class MutatorASTConsumer_479 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_479(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_template_constraint_with_false_479.h"

// ========================================================================================================
#define MUT479_OUTPUT 1

void MutatorFrontendAction_479::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto DL = MT->getTemplatedDecl();
      if (!DL->isFunctionOrFunctionTemplate())
        return;
      auto constraints = MT->getAssociatedConstraints();
      if (constraints.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      auto first_constraint = constraints.front();
      auto constraint_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                    first_constraint->getSourceRange());
      llvm::outs() << constraint_str << '\n';
      constraint_str = "false";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(first_constraint->getSourceRange(), constraint_str);
    }
}
  
void MutatorFrontendAction_479::MutatorASTConsumer_479::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}