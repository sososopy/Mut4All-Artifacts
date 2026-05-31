//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Replace_Constraint_Concept_Definition_222
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
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Constraint_Constraint_Definition_222.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT222_OUTPUT 1

void MutatorFrontendAction_222::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Concept = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concept")) {
      //Filter nodes in header files
      if (!Concept || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Concept->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   Concept->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t equalsIndex = declaration.find('=');
      if (equalsIndex != std::string::npos) {
        declaration.replace(equalsIndex + 1, declaration.size() - equalsIndex - 1, "nullptr");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Concept->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_222::MutatorASTConsumer_222::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConceptDecl().bind("Concept");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}