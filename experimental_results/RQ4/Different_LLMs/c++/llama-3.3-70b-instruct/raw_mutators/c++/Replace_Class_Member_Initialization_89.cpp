```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Class_Member_Initialization_89
 */ 
class MutatorFrontendAction_89 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(89)

private:
    class MutatorASTConsumer_89 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_89(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Class_Member_Initialization_89.h"

// ========================================================================================================
#define MUT89_OUTPUT 1

void MutatorFrontendAction_89::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = CD->getInit(0);
      if (!initList)
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      std::string initListStr = stringutils::rangetoStr(*(Result.SourceManager),
                                                         initList->getSourceRange());
      size_t pos = declaration.find(initListStr);
      if (pos != std::string::npos) {
        // Replace the initialization with a default initialization
        mutatedDeclaration.replace(pos, initListStr.length(), "()");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()), mutatedDeclaration);
    }
}

void MutatorFrontendAction_89::MutatorASTConsumer_89::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}