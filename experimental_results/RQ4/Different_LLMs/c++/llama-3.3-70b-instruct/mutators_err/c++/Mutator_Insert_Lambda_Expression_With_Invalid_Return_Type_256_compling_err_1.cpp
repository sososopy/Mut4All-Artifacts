//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Expression_With_Invalid_Return_Type_256
 */ 
class MutatorFrontendAction_Insert_Lambda_Expression_With_Invalid_Return_Type_256 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Insert_Lambda_Expression_With_Invalid_Return_Type_256)

private:
    class MutatorASTConsumer_Insert_Lambda_Expression_With_Invalid_Return_Type_256 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Lambda_Expression_With_Invalid_Return_Type_256(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Lambda_Expression_With_Invalid_Return_Type_256.h"

// ========================================================================================================
#define MUT_Insert_Lambda_Expression_With_Invalid_Return_Type_256_OUTPUT 1

void MutatorFrontendAction_Insert_Lambda_Expression_With_Invalid_Return_Type_256::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration.insert(declaration.find("{") + 1, "\n/*mut*/[&]() -> decltype(function_name) {\n");
      declaration.insert(declaration.rfind("}"), "\n}();\n");
      int choice = getrandom::getRandomIndex(1);
      //Replace the original AST node with the mutated one
      if (choice) {
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_Insert_Lambda_Expression_With_Invalid_Return_Type_256::MutatorASTConsumer_Insert_Lambda_Expression_With_Invalid_Return_Type_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}