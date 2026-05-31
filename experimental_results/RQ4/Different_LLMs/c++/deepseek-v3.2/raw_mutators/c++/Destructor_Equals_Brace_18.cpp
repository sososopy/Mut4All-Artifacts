//header file
#pragma once
#include "Mutator_base.h"

/**
 * Destructor_Equals_Brace_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)
private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      //Get the source code text of target node
      auto destructorText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (destructorText.find("= {}") != std::string::npos) {
        int choice = getrandom::getRandomIndex(2);
        std::string mutatedText;
        if (choice == 0) {
          mutatedText = destructorText.replace(destructorText.find("= {}"), 4, "= {;}");
        } else {
          mutatedText = destructorText.replace(destructorText.find("= {}"), 4, "= { }");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(DT->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxDestructorDecl().bind("Destructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}