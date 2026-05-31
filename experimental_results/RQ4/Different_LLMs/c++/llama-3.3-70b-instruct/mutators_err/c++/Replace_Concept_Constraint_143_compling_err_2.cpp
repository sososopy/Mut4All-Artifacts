//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_143
 */ 
class MutatorFrontendAction_143 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(143)

private:
    class MutatorASTConsumer_143 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_143(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Concept_Constraint_143.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT143_OUTPUT 1

void MutatorFrontendAction_143::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateParameterList>("TemplateParameterList")) {
      //Filter nodes in header files
      if (!MT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace concept constraint with a different concept constraint or remove it
      std::string newConstraint;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        // Replace with a different concept constraint
        newConstraint = "Foo";
      } else {
        // Remove the constraint
        newConstraint = "";
      }
      size_t pos = declaration.find("C");
      if (pos!= std::string::npos) {
        declaration.replace(pos, 1, newConstraint);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_143::MutatorASTConsumer_143::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxTemplateParameterList().bind("TemplateParameterList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}