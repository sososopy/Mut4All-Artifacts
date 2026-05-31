//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Concept_Requirement_91
 */ 
class MutatorFrontendAction_91 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(91)

private:
    class MutatorASTConsumer_91 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_91(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Concept_Requirement_91.h"

// ========================================================================================================
#define MUT91_OUTPUT 1

void MutatorFrontendAction_91::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptRange = CD->getSourceRange();
      std::string conceptText = stringutils::rangetoStr(*(Result.SourceManager), conceptRange);
      
      //Find requires-expression within the concept
      if (auto *RE = Result.Nodes.getNodeAs<clang::Expr>("RequiresExpr")) {
        //Check if this requires-expression is part of the concept definition
        if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RE->getBeginLoc()))
          return;
        
        //Look for template argument lists within the requires-expression
        auto requiresRange = RE->getSourceRange();
        std::string requiresText = stringutils::rangetoStr(*(Result.SourceManager), requiresRange);
        
        //Find template specialization expressions within the requires-expression
        for (auto *child : RE->children()) {
          if (auto *TSE = llvm::dyn_cast<clang::TemplateSpecializationType>(child->getTypePtr())) {
            //Get the template argument list
            auto templateArgs = TSE->getTemplateArgs();
            if (templateArgs.size() > 0) {
              //Mutate by removing template arguments or making them empty
              std::string mutatedText = requiresText;
              size_t argStart = mutatedText.find("<");
              size_t argEnd = mutatedText.find(">", argStart);
              if (argStart != std::string::npos && argEnd != std::string::npos) {
                //Replace template arguments with empty list or malformed reference
                int choice = getrandom::getRandomIndex(1);
                if (choice == 0) {
                  //Empty template argument list
                  mutatedText.replace(argStart + 1, argEnd - argStart - 1, "");
                } else {
                  //Malformed template argument referencing non-existent parameter
                  mutatedText.replace(argStart + 1, argEnd - argStart - 1, "NonExistentParam");
                }
                
                //Replace the requires-expression in the concept definition
                size_t requiresPos = conceptText.find(requiresText);
                if (requiresPos != std::string::npos) {
                  conceptText.replace(requiresPos, requiresText.length(), mutatedText);
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(conceptRange), conceptText);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_91::MutatorASTConsumer_91::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl(hasDescendant(expr().bind("RequiresExpr"))).bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}