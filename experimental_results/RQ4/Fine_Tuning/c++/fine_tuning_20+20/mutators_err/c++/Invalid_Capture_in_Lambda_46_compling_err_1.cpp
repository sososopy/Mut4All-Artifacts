//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Capture_in_Lambda_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> var_names;
    };
};

//source file
#include "../include/invalid_capture_in_lambda_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto var_name = DL->getNameAsString();
      var_names.push_back(var_name);
    } else if (auto *LE =
                   Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      auto capture_list = LE->capture_init_size();
      auto capture_string = stringutils::rangetoStr(
          *(Result.SourceManager), LE->getCaptureDefaultLoc(),
          LE->getEndLoc());
      llvm::outs() << capture_list << '\n';
      std::vector<string> filtered_var_names;
      for (auto var_name : var_names) {
        if (capture_string.find(var_name) == string::npos)
          filtered_var_names.push_back(var_name);
      }
      if (filtered_var_names.empty())
        return;
      auto random_index = getrandom::getRandomIndex(filtered_var_names.size());
      auto insert_capture = filtered_var_names[random_index];
      auto origin = stringutils::rangetoStr(*(Result.SourceManager),
                                            LE->getSourceRange());
      if (capture_list == 0) {
        origin.insert(origin.find(']'), insert_capture);
      } else {
        origin.insert(origin.find(']'), "," + insert_capture);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()),
                          origin);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("VarDecl");
    auto lambda_matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}