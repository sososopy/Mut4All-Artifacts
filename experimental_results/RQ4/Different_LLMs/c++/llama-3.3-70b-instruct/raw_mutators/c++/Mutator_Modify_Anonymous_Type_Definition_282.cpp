//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Anonymous_Type_Definition_282
 */ 
class MutatorFrontendAction_282 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(282)

private:
    class MutatorASTConsumer_282 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_282(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Anonymous_Type_Definition_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("RecordDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->getNameAsString() == "") {
        // Add a tag name to the anonymous type definition
        declaration.insert(0, "struct X ");
      } else {
        // Remove the tag name from the type definition
        size_t pos = declaration.find("struct");
        if (pos != string::npos) {
          size_t spacePos = declaration.find(" ", pos);
          if (spacePos != string::npos) {
            declaration.erase(pos + 7, spacePos - pos - 7);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl().bind("RecordDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}