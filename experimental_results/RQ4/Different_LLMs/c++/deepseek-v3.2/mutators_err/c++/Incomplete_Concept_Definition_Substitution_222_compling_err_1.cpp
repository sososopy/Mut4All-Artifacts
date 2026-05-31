//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Concept_Definition_Substitution_222
 */ 
class MutatorFrontendAction_222 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(222)

private:
    class MutatorASTConsumer_222 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_222(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Concept_Definition_Substitution_222.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptRange = MT->getSourceRange();
      auto conceptText = stringutils::rangetoStr(*(Result.SourceManager), conceptRange);
      //Find the constraint expression part
      auto constraintExpr = MT->getConstraintExpr();
      if (!constraintExpr) return;
      auto constraintRange = constraintExpr->getSourceRange();
      auto constraintText = stringutils::rangetoStr(*(Result.SourceManager), constraintRange);
      //Perform mutation on the source code text by applying string replacement
      string mutatedText = conceptText;
      mutatedText.replace(mutatedText.find(constraintText), constraintText.length(), "0");
      mutatedText = "/*mut222*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(conceptRange), mutatedText);
    }
}
  
void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}