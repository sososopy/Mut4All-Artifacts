//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Return_Type_To_Decltype_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/modify_lambda_return_type_to_decltype_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (DL->hasExplicitParameters() == false)
        return;
      if (DL->hasExplicitResultType() == false)
        return;
      auto RT = DL->getCallOperator()->getReturnType();
      auto return_type = RT.getAsString();
      if (return_type.find("decltype") != string::npos) {
        llvm::outs() << "already decltype\n";
        auto decltype_content = content.substr(
            content.find("decltype") + 9,
            content.find(')', content.find("decltype")) -
                content.find("decltype") - 9);
        llvm::outs() << decltype_content << '\n';
        auto new_decltype_content =
            stringutils::strReplace(decltype_content, "+", "-");
        llvm::outs() << new_decltype_content << '\n';
        content = stringutils::strReplace(content, decltype_content,
                                       new_decltype_content);
        llvm::outs() << content << '\n';
      } else {
        auto body_content = content.substr(content.find('{') + 1,
                                           content.rfind('}') -
                                               content.find('{') - 1);
        llvm::outs() << body_content << '\n';
        content = stringutils::strReplace(content, return_type,
                                       "decltype(" + body_content + ")");
        llvm::outs() << content << '\n';
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}