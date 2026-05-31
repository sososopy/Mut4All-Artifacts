//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Consteval_With_Constinit_96
 */ 
class MutatorFrontendAction_Replace_Consteval_With_Constinit_96 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Consteval_With_Constinit_96)

private:
    class MutatorASTConsumer_Replace_Consteval_With_Constinit_96 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Consteval_With_Constinit_96(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Consteval_With_Constinit_96.h"

// ========================================================================================================
#define REPLACE_CONSTEVAL_WITH_CONSTINIT_96_OUTPUT 1

void MutatorFrontendAction_Replace_Consteval_With_Constinit_96::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (FD->isConsteval()) {
        std::string mutatedDeclaration = declaration;
        size_t pos = mutatedDeclaration.find("consteval");
        if (pos != std::string::npos) {
          mutatedDeclaration.replace(pos, 9, "constinit");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_Replace_Consteval_With_Constinit_96::MutatorASTConsumer_Replace_Consteval_With_Constinit_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(constevalAttr())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}