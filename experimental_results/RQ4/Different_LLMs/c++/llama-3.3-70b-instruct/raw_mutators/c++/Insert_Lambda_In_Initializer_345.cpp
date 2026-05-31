```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_In_Initializer_345
 */ 
class MutatorFrontendAction_345 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(345)

private:
    class MutatorASTConsumer_345 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_345(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Lambda_In_Initializer_345.h"

// ========================================================================================================
#define MUT345_OUTPUT 1

void MutatorFrontendAction_345::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecls")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string lambdaExpression = "(4, []() { return [=]() { return " + FD->getNameAsString() + "; }(); }())";
      size_t pos = declaration.find("=");
      if (pos != std::string::npos) {
        declaration.replace(pos + 1, declaration.find(";") - pos - 1, lambdaExpression);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_345::MutatorASTConsumer_345::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInitializer(integerLiteral())).bind("FieldDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}