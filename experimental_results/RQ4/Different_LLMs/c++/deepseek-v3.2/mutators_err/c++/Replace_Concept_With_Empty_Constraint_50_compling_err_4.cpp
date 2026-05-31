//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_With_Empty_Constraint_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Concept_With_Empty_Constraint_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Get the source code text of target node
      auto constraintExpr = CD->getConstraintExpr();
      if (!constraintExpr)
         return;
      //Filter trivial constraints (true/false literals)
      if (llvm::dyn_cast<clang::CXXBoolLiteralExpr>(constraintExpr)) {
        return;
      }
      //Perform mutation on the source code text by applying string replacement
      //Get the source range from the concept keyword to the end of the constraint expression
      auto conceptLoc = CD->getBeginLoc();
      auto endLoc = constraintExpr->getEndLoc();
      //Adjust to include the '=' and constraint expression
      auto sourceRange = clang::SourceRange(conceptLoc, endLoc);
      //Get the original text
      auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Find the position of '=' to split
      auto eqPos = originalText.find('=');
      if (eqPos == std::string::npos)
        return;
      //Construct mutated text: keep everything before '=' and add semicolon
      auto mutatedText = originalText.substr(0, eqPos) + ";";
      mutatedText = "/*mut50*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl(hasName("")).bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}