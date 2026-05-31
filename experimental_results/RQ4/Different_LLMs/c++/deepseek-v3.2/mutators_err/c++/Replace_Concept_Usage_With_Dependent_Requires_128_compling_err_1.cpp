//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Usage_With_Dependent_Requires_128
 */ 
class MutatorFrontendAction_128 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(128)

private:
    class MutatorASTConsumer_128 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_128(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Concept_Usage_With_Dependent_Requires_128.h"

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ES = Result.Nodes.getNodeAs<clang::Expr>("ConceptExpr")) {
      //Filter nodes in header files
      if (!ES || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ES->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ES->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      // Find enclosing function template
      auto Parents = Result.Context->getParents(*ES);
      clang::FunctionTemplateDecl* FT = nullptr;
      for (auto& Parent : Parents) {
        if (auto* FTD = Parent.get<clang::FunctionTemplateDecl>()) {
          FT = FTD;
          break;
        }
      }
      if (!FT) return;
      
      // Get template parameters
      auto* TPT = FT->getTemplateParameters();
      if (!TPT || TPT->size() == 0) return;
      
      // Choose a template parameter
      int paramIndex = getrandom::getRandomIndex(TPT->size() - 1);
      auto* TP = TPT->getParam(paramIndex);
      std::string paramName = TP->getNameAsString();
      
      // Build requires-expression
      std::string requiresExpr = "requires { typename " + paramName + "::type; } ";
      
      // Replace original concept expression
      std::string mutatedExpr = requiresExpr + conceptExpr;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ES->getSourceRange()), mutatedExpr);
    }
}
  
void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = expr(hasAncestor(functionTemplateDecl()), unless(hasAncestor(compoundStmt()))).bind("ConceptExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}