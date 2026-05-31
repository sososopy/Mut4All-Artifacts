//header file
#pragma once
#include "Mutator_base.h"

/**
 * Append_Recursive_Call_To_Lambda_Trailing_Return_Type_169
 */ 
class MutatorFrontendAction_169 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(169)

private:
    class MutatorASTConsumer_169 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_169(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Append_Recursive_Call_To_Lambda_Trailing_Return_Type_169.h"

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("->") != string::npos &&
          content.find("return") != string::npos) {
        auto param = content.substr(content.find('['), content.find(']') + 1);
        if (param.find('&') == string::npos) {
          if (param.size() == 2)
            param.insert(1, "&");
          else
            param.insert(1, "&,");
        }
        auto lambda_name = content.substr(0, content.find('='));
        lambda_name = stringutils::trim(lambda_name);
        auto ret_type = content.substr(content.find("->"));
        ret_type = ret_type.substr(0, ret_type.find('{'));
        if (ret_type.find(lambda_name) == string::npos) {
          ret_type.insert(ret_type.rfind(')'), "+" + lambda_name + "(x)");
        }
        content = param + ret_type +
                  content.substr(content.find('{'), content.size());
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(LT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}