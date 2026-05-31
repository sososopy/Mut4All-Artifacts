//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_With_Inherited_Constraint_109
 */ 
class MutatorFrontendAction_109 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(109)

private:
    class MutatorASTConsumer_109 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_109(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_With_Inherited_Constraint_109.h"

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
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
      std::string newDeclaration = declaration;
      size_t pos = newDeclaration.find("convertible_to");
      if (pos != std::string::npos) {
        newDeclaration.replace(pos, 13, "std::same_as");
      } else {
        pos = newDeclaration.find("convertible_to");
        if (pos != std::string::npos) {
          newDeclaration.replace(pos, 13, "std::derived_from");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newDeclaration);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(hasRequiresClause()).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}