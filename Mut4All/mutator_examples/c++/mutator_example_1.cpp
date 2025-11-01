//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Expression_1
 */ 

class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)
private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_lambda_expression_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody() && !MT->isVirtual()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration.insert(declaration.find("{") + 1, "\n/*mut9*/[&](){\n");
      declaration.insert(declaration.rfind("}"), "\n}();\n");
      int choice = getrandom::getRandomIndex(1);
      //Replace the original AST node with the mutated one
      if (choice) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
  }
}
  
void MutatorFrontendAction__1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}