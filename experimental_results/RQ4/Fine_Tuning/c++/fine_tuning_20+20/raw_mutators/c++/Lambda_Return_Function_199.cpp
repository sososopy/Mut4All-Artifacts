//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Return_Function_199
 */ 
class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)

private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *current_lambda;
    };
};

//source file
#include "../include/lambda_return_function_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      current_lambda = DL->getLambdaClass();
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("return") != string::npos) {
        if (content.find("->") != string::npos) {
          content = content.substr(0, content.find("->") + 2) + "int(int)" +
                    content.substr(content.find(")"));
        } else {
          content = content.substr(0, content.find(')')) + "->int(int)" +
                    content.substr(content.find(')'));
        }
        if (content.find("return") != string::npos) {
          content.insert(content.find("return"), "int y=0;");
        }
        llvm::outs() << content << '\n';
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
      }
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "LambdaClasses")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (CL != current_lambda)
        return;
      if (CL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      if (content.find("operator()") != string::npos) {
        auto op_pos = content.find("operator()");
        if (content.find("int(int)") == string::npos) {
          content = content.substr(0, op_pos) + "int(int) " +
                    content.substr(op_pos);
        }
      }
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto lambda_class_matcher =
        cxxRecordDecl(isLambda()).bind("LambdaClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_class_matcher, &callback);
    matchFinder.matchAST(Context);
}