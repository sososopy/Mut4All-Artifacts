//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Parameter_With_Fp16_In_Template_320
 */ 
class MutatorFrontendAction_320 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(320)

private:
    class MutatorASTConsumer_320 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_320(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/Replace_Lambda_Parameter_With_Fp16_In_Template_320.h"

// ========================================================================================================
#define MUT320_OUTPUT 1

void MutatorFrontendAction_320::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getBeginLoc()))
        return;
      llvm::outs() << "Mut320: Lambda detected\n";
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LT->getSourceRange());
      auto params = LT->getLambdaClass()->getLambdaCallOperator()->parameters();
      if (params.empty()) {
        content.insert(content.find(')'), "__fp16");
      } else {
        int choice = getrandom::getRandomIndex(params.size() - 1);
        auto target = params[choice];
        auto target_content = stringutils::rangetoStr(
            *(Result.SourceManager), target->getSourceRange());
        llvm::outs() << "Mut320: Target parameter: " << target_content << '\n';
        stringutils::strReplace(target_content, target->getType().getAsString(),
                                "__fp16");
        llvm::outs() << "Mut320: Replaced parameter: " << target_content
                     << '\n';
        stringutils::strReplace(content, target->getNameAsString(),
                                target_content);
      }
      llvm::outs() << "Mut320: Lambda replaced: " << content << '\n';
      Rewrite.ReplaceText(LT->getSourceRange(), content);
    } else if (auto *TP = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                   "Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getBeginLoc()))
        return;
      cur_templates.push_back(TP);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CallExpr>("Calls")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      auto callee = CL->getDirectCallee();
      if (callee == nullptr)
        return;
      auto name = callee->getNameAsString();
      for (auto target : cur_templates) {
        if (target->getNameAsString() == name) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CL->getSourceRange());
          llvm::outs() << "Mut320: Target call: " << content << '\n';
          content.insert(0, "/*mut320*/");
          Rewrite.ReplaceText(CL->getSourceRange(), content);
        }
      }
    }
  }
  
void MutatorFrontendAction_320::MutatorASTConsumer_320::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto template_matcher = functionTemplateDecl().bind("Templates");
    auto call_matcher = callExpr().bind("Calls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.matchAST(Context);
}