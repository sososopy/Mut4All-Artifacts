//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_Type_36
 */ 
class MutatorFrontendAction_36 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_36>(TheRewriter);
    }

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
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Lambda_Capture_Type_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambda")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isInMainFile(L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto capture = L->capture_begin();
      if (capture == L->capture_end())
        return;
      std::string new_capture;
      unsigned i = 0;
      for (; capture != L->capture_end(); ++capture, ++i) {
        auto cap = *capture;
        if (cap.getKind() == clang::LambdaCapture::Init) {
          auto init = cap.getInit();
          auto var = init->getVariable();
          auto type = var->getType();
          if (type->isPointerType()) {
            new_capture += "&";
          } else if (type->isReferenceType()) {
            new_capture += "=";
          } else if (type->isConstantType()) {
            new_capture += "= std::move";
          } else {
            new_capture += "=";
          }
          new_capture += var->getNameAsString();
        } else if (cap.getKind() == clang::LambdaCapture::ByValue) {
          new_capture += "=";
          new_capture += cap.getVariable()->getNameAsString();
        } else if (cap.getKind() == clang::LambdaCapture::ByReference) {
          new_capture += "&";
          new_capture += cap.getVariable()->getNameAsString();
        }
        if (i != L->capture_size() - 1)
          new_capture += ", ";
      }
      //Perform mutation on the source code text by applying string replacement
      auto lambda_text = stringutils::rangetoStr(*(Result.SourceManager), L->getSourceRange());
      size_t pos = lambda_text.find('[');
      lambda_text.replace(pos + 1, lambda_text.find(']') - pos - 1, new_capture);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(L->getSourceRange()), lambda_text);
    }
}
  
void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}