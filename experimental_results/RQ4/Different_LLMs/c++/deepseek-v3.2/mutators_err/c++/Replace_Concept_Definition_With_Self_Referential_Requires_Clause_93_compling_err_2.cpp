//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Definition_With_Self_Referential_Requires_Clause_93
 */ 
class MutatorFrontendAction_93 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(93)

private:
    class MutatorASTConsumer_93 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_93(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> variableNames;
    };
};

//source file
#include "../include/Mutator_Replace_Concept_Definition_With_Self_Referential_Requires_Clause_93.h"

// ========================================================================================================
#define MUT93_OUTPUT 1

void MutatorFrontendAction_93::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptName = MT->getNameAsString();
      
      //Collect variable names from the seed program for potential parameter name
      auto &SM = *Result.SourceManager;
      auto &Context = *Result.Context;
      auto decls = Context.getTranslationUnitDecl()->decls();
      for (auto decl : decls) {
        if (auto varDecl = dyn_cast<VarDecl>(decl)) {
          if (SM.isWrittenInMainFile(varDecl->getLocation())) {
            variableNames.push_back(varDecl->getNameAsString());
          }
        }
      }
      
      //Determine parameter name
      std::string paramName = "x";
      if (!variableNames.empty()) {
        paramName = variableNames[getrandom::getRandomIndex(variableNames.size() - 1)];
      }
      
      //Get original requires clause source
      auto requiresRange = MT->getSourceRange();
      auto requiresText = stringutils::rangetoStr(SM, requiresRange);
      
      //Find parameter list part
      auto paramListStart = requiresText.find('(');
      auto paramListEnd = requiresText.find(')', paramListStart);
      if (paramListStart == std::string::npos || paramListEnd == std::string::npos) return;
      
      //Construct new parameter list
      std::string newParamList = conceptName + " auto " + paramName;
      if (paramListEnd - paramListStart > 1) {
        //Original has parameters, insert before existing ones
        newParamList += ", ";
        requiresText.insert(paramListStart + 1, newParamList);
      } else {
        //No parameters, replace empty parentheses
        requiresText.replace(paramListStart, paramListEnd - paramListStart + 1, "(" + newParamList + ")");
      }
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(requiresRange), requiresText);
    }
}
  
void MutatorFrontendAction_93::MutatorASTConsumer_93::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}