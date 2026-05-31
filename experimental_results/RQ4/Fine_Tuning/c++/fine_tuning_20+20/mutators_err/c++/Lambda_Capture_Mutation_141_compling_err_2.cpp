//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_Mutation_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> surrounding_vars;
    };
};

//source file
#include "../include/Lambda_Capture_Mutation_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LB = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LB || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LB->getBeginLoc()))
        return;
      auto capture_kind = LB->getCaptureDefault();
      if (capture_kind == LambdaCaptureDefault::LCD_None)
        return;
      if (capture_kind == LambdaCaptureDefault::LCD_ByCopy) {
        llvm::outs() << "=\n";
      } else if (capture_kind == LambdaCaptureDefault::LCD_ByRef) {
        llvm::outs() << "&\n";
      }
      std::vector<const clang::VarDecl *> lambda_vars;
      auto captures = LB->captures();
      for (auto capture : captures) {
        if (capture.capturesVariable()) {
          lambda_vars.push_back(capture.getCapturedVar());
        }
      }
      if (lambda_vars.empty()) {
        llvm::outs() << "empty lambda\n";
        return;
      }
      std::vector<const clang::VarDecl *> candidate_vars;
      for (auto var : surrounding_vars) {
        bool flag = true;
        for (auto lambda_var : lambda_vars) {
          if (var->getNameAsString() == lambda_var->getNameAsString()) {
            flag = false;
            break;
          }
        }
        if (flag)
          candidate_vars.push_back(var);
      }
      if (candidate_vars.empty()) {
        llvm::outs() << "no candidate\n";
        return;
      }
      size_t index = getrandom::getRandomIndex(lambda_vars.size() - 1);
      size_t candidate_index = getrandom::getRandomIndex(candidate_vars.size() - 1);
      auto origin_name = lambda_vars[index]->getNameAsString();
      auto candidate_name = candidate_vars[candidate_index]->getNameAsString();
      llvm::outs() << "origin: " << origin_name << '\n';
      llvm::outs() << "candidate: " << candidate_name << '\n';
      auto origin_capture = stringutils::rangetoStr(*(Result.SourceManager),
                                                    lambda_vars[index]->getSourceRange());
      llvm::outs() << "origin_capture: " << origin_capture << '\n';
      auto origin_capture_kind = LB->captures()[index].getCaptureKind();
      if (origin_capture_kind == LambdaCaptureKind::LCK_ByRef)
        origin_capture = "&" + origin_capture;
      else if (origin_capture_kind == LambdaCaptureKind::LCK_ByCopy)
        origin_capture = "=" + origin_capture;
      llvm::outs() << "origin_capture: " << origin_capture << '\n';
      auto candidate_capture = stringutils::rangetoStr(*(Result.SourceManager),
                                                       candidate_vars[candidate_index]->getSourceRange());
      auto candidate_capture_kind = LB->captures()[candidate_index].getCaptureKind();
      if (candidate_capture_kind == LambdaCaptureKind::LCK_ByRef)
        candidate_capture = "&" + candidate_capture;
      else if (candidate_capture_kind == LambdaCaptureKind::LCK_ByCopy)
        candidate_capture = "=" + candidate_capture;
      llvm::outs() << "candidate_capture: " << candidate_capture << '\n';
      auto origin_capture_list =
          stringutils::rangetoStr(*(Result.SourceManager), LB->getCaptureInit()->getSourceRange());
      llvm::outs() << "origin_capture_list: " << origin_capture_list << '\n';
      string new_capture_list = "";
      for (size_t i = 0; i < origin_capture_list.size();) {
        if (origin_capture_list[i] == origin_capture[0]) {
          if (origin_capture_list.substr(i, origin_capture.size()) ==
              origin_capture) {
            new_capture_list += candidate_capture;
            i += origin_capture.size();
          } else {
            new_capture_list += origin_capture_list[i];
            i++;
          }
        } else {
          new_capture_list += origin_capture_list[i];
          i++;
        }
      }
      llvm::outs() << "new_capture_list: " << new_capture_list << '\n';
      Rewrite.ReplaceText(LB->getCaptureInit()->getSourceRange(), new_capture_list);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Vars")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getBeginLoc()))
        return;
      surrounding_vars.push_back(VD);
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto lambda_matcher = lambdaExpr().bind("Lambdas");
    auto var_matcher = varDecl().bind("Vars");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambda_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}