//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Concept_Constraint_In_Function_Template_110
 */ 
class MutatorFrontendAction_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)

private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Concept_Constraint_In_Function_Template_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_110::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto DL = MT->getTemplatedDecl();
      if (!DL->hasBody())
        return;
      auto requires_clause = DL->getTrailingRequiresClause();
      if (!requires_clause)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             requires_clause->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto concept_names = stringutils::getAllType(content);
      if (concept_names.size() == 0)
        return;
      auto concept_name =
          concept_names[getrandom::getRandomIndex(concept_names.size() - 1)];
      auto new_concept_name = getrandom::getRandomType();
      llvm::outs() << concept_name << " " << new_concept_name << "\n";
      stringutils::replace_all(content, concept_name, new_concept_name);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              requires_clause->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_110::MutatorASTConsumer_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}