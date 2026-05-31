//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_With_Defaulted_Lambda_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_concept_constraint_with_defaulted_lambda_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParameter")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto concept = MT->getConstraintExpression();
      if (concept == nullptr)
        return;
      auto concept_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                 concept->getSourceRange());
      llvm::outs() << concept_str << '\n';
      auto lambda =
          "decltype([](" + name + " mut112) { return " + concept_str + "; })";
      //Perform mutation on the source code text by applying string replacement
      auto origin = stringutils::rangetoStr(*(Result.SourceManager),
                                            MT->getSourceRange());
      auto target = origin + "=" + lambda;
      target = "/*mut112*/" + target;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), target);
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(hasTypeConstraint()).bind("TemplateParameter");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}