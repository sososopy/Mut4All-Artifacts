//header file
#pragma once
#include "Mutator_base.h"

/**
 * ChangeConstructorInitialization_187
 */ 
class MutatorFrontendAction_ChangeConstructorInitialization_187 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(ChangeConstructorInitialization_187)

private:
    class MutatorASTConsumer_ChangeConstructorInitialization_187 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_ChangeConstructorInitialization_187(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ChangeConstructorInitialization_187.h"

// ========================================================================================================
#define CHANGECONSTRUCTORINITIALIZATION_187_OUTPUT 1

void MutatorFrontendAction_ChangeConstructorInitialization_187::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Change the order of initialization
      if (MT->getNumParams() > 1) {
        std::string param1 = MT->getParamDecl(0)->getNameAsString();
        std::string param2 = MT->getParamDecl(1)->getNameAsString();
        size_t pos = declaration.find(param1);
        size_t pos2 = declaration.find(param2);
        if (pos != std::string::npos && pos2 != std::string::npos) {
          declaration.replace(pos, param1.length(), param2);
          declaration.replace(pos2, param2.length(), param1);
        }
      }
      // Change the parameters of constructor calls
      else if (MT->getNumParams() > 0) {
        std::string param = MT->getParamDecl(0)->getNameAsString();
        size_t pos = declaration.find(param);
        if (pos != std::string::npos) {
          declaration.replace(pos, param.length(), "0"); // Replace with a constant value
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_ChangeConstructorInitialization_187::MutatorASTConsumer_ChangeConstructorInitialization_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}