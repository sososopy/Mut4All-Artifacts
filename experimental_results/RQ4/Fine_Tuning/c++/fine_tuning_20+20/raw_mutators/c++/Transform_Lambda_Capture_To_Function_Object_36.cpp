//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Lambda_Capture_To_Function_Object_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(36)

private:
    class MutatorASTConsumer_36 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_36(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::LambdaExpr *> lambda_list;
    };
};

//source file
#include "../include/Transform_Lambda_Capture_To_Function_Object_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      lambda_list.push_back(LE);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclRefExpr>("DeclRef")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto DL_name = DL->getNameInfo().getAsString();
      llvm::outs() << DL_name << '\n';
      for (auto lambda : lambda_list) {
        auto captures = lambda->getLambdaClass()->captures();
        for (auto capture : captures) {
          if (capture.capturesVariable() &&
              capture.getCapturedVar()->getNameAsString() == DL_name) {
            auto DL_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                   DL->getSourceRange());
            DL_text += "_fn()";
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(DL->getSourceRange()), DL_text);
          }
        }
      }
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "LambdaClass")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      if (!CL->isLambda())
        return;
      auto lambda = CL->getLambdaContextDecl();
      llvm::outs() << "lambda: " << lambda->getNameAsString() << '\n';
      auto captures = CL->captures();
      for (auto capture : captures) {
        if (capture.capturesVariable()) {
          auto DL = capture.getCapturedVar();
          auto DL_name = DL->getNameAsString();
          llvm::outs() << DL_name << '\n';
          string DL_type = DL->getType().getAsString();
          if (DL->getType()->isLValueReferenceType() ||
              DL->getType()->isRValueReferenceType()) {
            DL_type = DL->getType().getPointeeType().getAsString();
          }
          string fn_name = DL_name + "_fn";
          string fn_decl = "auto " + fn_name + "=[]()->" + DL_type + "&{";
          fn_decl += "return " + DL_name + ";};";
          fn_decl = "/*mut36*/" + fn_decl;
          llvm::outs() << fn_decl << '\n';
          Rewrite.ReplaceText(DL->getEndLoc(), 0, fn_decl);
        }
      }
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambda");
    auto declref_matcher = declRefExpr().bind("DeclRef");
    auto lambda_class_matcher = cxxRecordDecl().bind("LambdaClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(declref_matcher, &callback);
    matchFinder.addMatcher(lambda_class_matcher, &callback);
    matchFinder.matchAST(Context);
}