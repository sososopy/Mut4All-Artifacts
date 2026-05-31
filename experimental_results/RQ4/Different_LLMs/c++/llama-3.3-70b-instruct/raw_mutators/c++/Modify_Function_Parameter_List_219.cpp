//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Parameter_List_219
 */ 
class MutatorFrontendAction_219 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(219)

private:
    class MutatorASTConsumer_219 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_219(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Function_Parameter_List_219.h"

// ========================================================================================================
#define MUT219_OUTPUT 1

void MutatorFrontendAction_219::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> params;
      for (auto param : FD->params()) {
        params.push_back(param->getOriginalType().getAsString() + " " + param->getNameAsString());
      }
      int choice = getrandom::getRandomIndex(3);
      if (choice == 0) { // Add a new parameter
        std::string newParamType = getrandom::getRandomType();
        std::string newParamName = getrandom::getRandomName();
        params.push_back(newParamType + " " + newParamName);
      } else if (choice == 1) { // Remove a parameter
        if (!params.empty()) {
          params.erase(params.begin() + getrandom::getRandomIndex(params.size()));
        }
      } else { // Modify an existing parameter
        if (!params.empty()) {
          int index = getrandom::getRandomIndex(params.size());
          std::string newParamType = getrandom::getRandomType();
          params[index] = newParamType + " " + params[index].substr(params[index].find(' ') + 1);
        }
      }
      std::string mutatedParams;
      for (auto param : params) {
        mutatedParams += param + ", ";
      }
      if (!mutatedParams.empty()) {
        mutatedParams.pop_back();
        mutatedParams.pop_back();
      }
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find('(');
      mutatedDeclaration.replace(pos + 1, declaration.find(')') - pos - 1, mutatedParams);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_219::MutatorASTConsumer_219::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}