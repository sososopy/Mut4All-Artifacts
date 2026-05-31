//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Placeholder_With_Undeclared_Identifier_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(387)

private:
    class MutatorASTConsumer_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_387(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> declaredIdentifiers;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Parameter_Placeholder_With_Undeclared_Identifier_387.h"

// ========================================================================================================
#define MUT387_OUTPUT 1

void MutatorFrontendAction_387::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto *TPL = TD->getTemplateParameters();
      if (!TPL || TPL->size() == 0)
        return;
      
      //Collect declared identifiers from the translation unit
      if (declaredIdentifiers.empty()) {
        auto &SM = *Result.SourceManager;
        auto &Context = *Result.Context;
        for (auto *D : Context.getTranslationUnitDecl()->decls()) {
          if (auto *ND = dyn_cast<NamedDecl>(D)) {
            if (SM.isWrittenInMainFile(ND->getLocation())) {
              declaredIdentifiers.push_back(ND->getNameAsString());
            }
          }
        }
      }
      
      //Find a template parameter with placeholder and identifier
      for (unsigned i = 0; i < TPL->size(); ++i) {
        auto *TP = TPL->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(TP)) {
          if (TTP->hasName()) {
            std::string oldName = TTP->getNameAsString();
            
            //Generate candidate undeclared identifiers (single lowercase letters)
            std::vector<std::string> candidates;
            for (char c = 'a'; c <= 'z'; ++c) {
              std::string candidate(1, c);
              if (std::find(declaredIdentifiers.begin(), declaredIdentifiers.end(), candidate) == declaredIdentifiers.end()) {
                candidates.push_back(candidate);
              }
            }
            
            if (candidates.empty()) {
              continue;
            }
            
            //Randomly select an undeclared identifier
            int idx = getrandom::getRandomIndex(candidates.size() - 1);
            std::string newName = candidates[idx];
            
            //Perform mutation on the source code text by applying string replacement
            SourceLocation nameLoc = TTP->getLocation();
            if (nameLoc.isValid()) {
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(nameLoc, oldName.length(), newName);
              return; // Mutate only one parameter per template declaration
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}