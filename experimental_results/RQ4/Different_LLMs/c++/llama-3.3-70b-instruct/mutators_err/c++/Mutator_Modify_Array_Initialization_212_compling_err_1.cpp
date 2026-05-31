```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Array_Initialization_212
 */ 
class MutatorFrontendAction_212 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(212)

private:
    class MutatorASTConsumer_212 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_212(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Array_Initialization_212.h"

// ========================================================================================================
#define MUT212_OUTPUT 1

void MutatorFrontendAction_212::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VA = Result.Nodes.getNodeAs<clang::VarDecl>("VarArray")) {
      //Filter nodes in header files
      if (!VA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VA->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VA->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("{") != string::npos && declaration.find("}") != string::npos) {
        int choice = getrandom::getRandomIndex(3);
        if (choice == 0) {
          // Add a new initializer clause
          size_t pos = declaration.find("}");
          declaration.insert(pos, ", 0");
        } else if (choice == 1) {
          // Remove an existing initializer clause
          size_t pos = declaration.find(",");
          if (pos != string::npos) {
            size_t endPos = declaration.find("}", pos);
            declaration.erase(pos, endPos - pos);
          }
        } else {
          // Replace an existing initializer clause with a new one
          size_t pos = declaration.find(",");
          if (pos != string::npos) {
            size_t endPos = declaration.find(",", pos + 1);
            if (endPos == string::npos) {
              endPos = declaration.find("}", pos);
            }
            declaration.replace(pos + 1, endPos - pos - 1, "0");
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VA->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_212::MutatorASTConsumer_212::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(arrayType())).bind("VarArray");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}