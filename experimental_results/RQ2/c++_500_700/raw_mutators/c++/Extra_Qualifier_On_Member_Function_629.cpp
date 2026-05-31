//header file
#pragma once
#include "Mutator_base.h"

/**
 * extra_qualifier_on_member_function_629
 */ 
class MutatorFrontendAction_629 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(629)

private:
    class MutatorASTConsumer_629 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_629(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/extra_qualifier_on_member_function_629.h"

// ========================================================================================================
#define MUT629_OUTPUT 1

void MutatorFrontendAction_629::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto classDecl = MT->getParent();
      std::string className = classDecl->getNameAsString();
      std::string methodName = MT->getNameAsString();
      std::string qualifiedName = className + "::" + methodName;
      //Perform mutation on the source code text by applying string replacement
      Rewrite.ReplaceText(MT->getNameInfo().getSourceRange(), qualifiedName);
    }
}
  
void MutatorFrontendAction_629::MutatorASTConsumer_629::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isDefinition()).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}