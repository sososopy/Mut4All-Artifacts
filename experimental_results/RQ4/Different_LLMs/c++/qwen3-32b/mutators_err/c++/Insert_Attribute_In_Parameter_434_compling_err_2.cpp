//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Attribute_In_Parameter_434
 */ 
class MutatorFrontendAction_Insert_Attribute_In_Parameter_434 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(434)

private:
    class MutatorASTConsumer_434 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_434(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/insert_attribute_in_parameter_434.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT434_OUTPUT 1

void MutatorFrontendAction_Insert_Attribute_In_Parameter_434::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ParmVarDecl>("param")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto paramText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t lastSpace = paramText.find_last_of(' ');
      if (lastSpace == string::npos) return;
      string typePart = paramText.substr(0, lastSpace);
      string namePart = paramText.substr(lastSpace + 1);
      int choice = getrandom::getRandomIndex(2);
      string attribute;
      if (choice == 0) {
          attribute = "[[nodiscard]]";
      } else {
          attribute = "[[deprecated]]";
      }
      string newParamText = typePart + " " + attribute + " " + namePart;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), newParamText);
    }
}
  
void MutatorFrontendAction_Insert_Attribute_In_Parameter_434::MutatorASTConsumer_434::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = parmVarDecl(parent(functionDecl())).bind("param");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}