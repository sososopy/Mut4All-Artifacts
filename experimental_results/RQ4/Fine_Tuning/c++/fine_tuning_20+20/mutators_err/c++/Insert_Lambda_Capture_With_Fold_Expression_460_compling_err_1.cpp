//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Lambda_Capture_With_Fold_Expression_460
 */ 
class MutatorFrontendAction_460 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(460)

private:
    class MutatorASTConsumer_460 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_460(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> cur_lambdas;
    };
};

//source file
#include "../include/insert_lambda_capture_with_fold_expression_460.h"

// ========================================================================================================
#define MUT460_OUTPUT 1

void MutatorFrontendAction_460::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (!FD->isTemplateInstantiation())
      return;
    if (!FD->isFunctionTemplateSpecialization())
      return;

    auto TSI = FD->getTemplateSpecializationInfo();
    if (!TSI)
      return;
    auto TPL = TSI->getTemplate();
    if (!TPL)
      return;
    if (TPL->getTemplateParameters() == nullptr)
      return;
    auto TP = TPL->getTemplateParameters();
    bool has_pack = false;
    for (auto param : *TP) {
      if (param->isTemplateParameterPack()) {
        has_pack = true;
        break;
      }
    }
    if (has_pack == false)
      return;
    llvm::outs() << "mut460: " << FD->getNameAsString() << '\n';
    auto DL = FD->getBody();
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find("...") != string::npos)
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
  } else if (auto *LD = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   LD->getBeginLoc()))
      return;
    cur_lambdas.push_back(LD);
  } else if (auto *RS = Result.Nodes.getNodeAs<clang::ReturnStmt>(
                 "Return0Stmt")) { // If there is a return 0 statement in the function, replace the return value with the inserted static const
    if (!RS || !Result.Context->getSourceManager().isWrittenInMainFile(
                   RS->getBeginLoc()))
      return;
    auto DL = RS->getDirectCallee();
    if (DL == nullptr)
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (cur_lambdas.empty())
      return;
    auto index = getrandom::getRandomIndex(cur_lambdas.size() - 1);
    auto target = cur_lambdas[index];
    auto target_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  target->getSourceRange());
    if (target_content.find("...") == string::npos)
      return;
    auto ins = "/*mut460*/return ([&]{}(),...,0)";
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(RS->getSourceRange()),
                        ins);
  }
}

void MutatorFrontendAction_460::MutatorASTConsumer_460::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto return0Stmt_macher =
        returnStmt(hasReturnValue(integerLiteral(equals(0))))
            .bind("Return0Stmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(return0Stmt_macher, &callback);
    matchFinder.matchAST(Context);
}