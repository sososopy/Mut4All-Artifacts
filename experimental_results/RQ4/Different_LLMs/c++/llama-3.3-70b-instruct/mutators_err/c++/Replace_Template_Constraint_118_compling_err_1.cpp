```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Constraint_118
 */ 
class MutatorFrontendAction_118 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(118)

private:
    class MutatorASTConsumer_118 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_118(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Template_Constraint_118.h"

// ========================================================================================================
#define MUT118_OUTPUT 1

void MutatorFrontendAction_118::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the constraint with a different constraint
      auto constraintStart = declaration.find("requires");
      if (constraintStart != string::npos) {
        auto constraintEnd = declaration.find(")", constraintStart);
        if (constraintEnd != string::npos) {
          auto constraint = declaration.substr(constraintStart + 7, constraintEnd - constraintStart - 7);
          //Replace the constraint with a different constraint
          auto newConstraint = "sizeof(T) > 0";
          declaration.replace(constraintStart + 7, constraint.length(), newConstraint);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_118::MutatorASTConsumer_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}