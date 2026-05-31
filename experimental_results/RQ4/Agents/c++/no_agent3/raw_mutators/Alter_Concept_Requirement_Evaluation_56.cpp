//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Concept_Requirement_Evaluation_56
 */ 
class MutatorFrontendAction_56 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(56)

private:
    class MutatorASTConsumer_56 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_56(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Alter_Concept_Requirement_Evaluation_56.h"

// ========================================================================================================
#define MUT56_OUTPUT 1

void MutatorFrontendAction_56::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newConcept = "ambiguous";
      std::string oldConcept = "matches";
      size_t pos = declaration.find(oldConcept);
      if (pos != std::string::npos) {
        declaration.replace(pos, oldConcept.length(), newConcept);
        declaration.insert(declaration.find("{") + 1, "\n/*mut56*/");
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_56::MutatorASTConsumer_56::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasRequiresClause()).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}