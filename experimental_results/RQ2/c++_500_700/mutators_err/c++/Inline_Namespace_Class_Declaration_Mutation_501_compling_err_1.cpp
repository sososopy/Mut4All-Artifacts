//header file
#pragma once
#include "Mutator_base.h"

/**
 * inline_namespace_class_declaration_mutation_501
 */ 
class MutatorFrontendAction_501 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(501)

private:
    class MutatorASTConsumer_501 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_501(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/inline_namespace_class_declaration_mutation_501.h"

// ========================================================================================================
#define MUT501_OUTPUT 1

void MutatorFrontendAction_501::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      if (CD->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto className = CD->getNameAsString();
      if (className.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string newClassDef = "class " + className + " { public: void sampleFunction(); };";
      Rewrite.ReplaceText(CD->getSourceRange(), newClassDef);

      // Insert the sampleFunction definition outside
      std::string funcDef = "void " + CD->getQualifiedNameAsString() + "::sampleFunction() { /* Function implementation */ }";
      Rewrite.InsertTextAfterToken(CD->getEndLoc(), "\n" + funcDef);
    }
}
  
void MutatorFrontendAction_501::MutatorASTConsumer_501::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(
        isInInlineNamespace(),
        unless(isDefinition())
    ).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}