```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Format_Attribute_Index_321
 */ 
class MutatorFrontendAction_321 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(321)

private:
    class MutatorASTConsumer_321 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_321(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Format_Attribute_Index_321.h"

// ========================================================================================================
#define MUT321_OUTPUT 1

void MutatorFrontendAction_321::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto attrs = FD->attrs();
      for (auto attr : attrs) {
        if (auto formatAttr = dyn_cast<clang::FormatAttr>(attr)) {
          int paramCount = FD->getNumParams();
          int newIndex = -1;
          if (paramCount > 0) {
            newIndex = paramCount + 1; // exceed the number of parameters
          } else {
            newIndex = 0; // set to 0 if the function has no parameters
          }
          //Perform mutation on the source code text by applying string replacement
          std::string mutatedAttr = "__attribute__((__format_arg__(" + std::to_string(newIndex) + ")))";
          Rewrite.ReplaceText(attr->getSourceRange(), mutatedAttr);
        }
      }
    }
}
  
void MutatorFrontendAction_321::MutatorASTConsumer_321::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(formatAttr())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}