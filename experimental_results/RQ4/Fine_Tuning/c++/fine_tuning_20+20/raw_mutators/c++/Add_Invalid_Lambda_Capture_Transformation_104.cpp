//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Invalid_Lambda_Capture_Transformation_104
 */ 
class MutatorFrontendAction_104 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(104)

private:
    class MutatorASTConsumer_104 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_104(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Invalid_Lambda_Capture_Transformation_104.h"

// ========================================================================================================
#define MUT104_OUTPUT 1

void MutatorFrontendAction_104::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      if (LE->getCaptureDefault() != LambdaCaptureKind::LCK_ByCopy)
        return;
      cur_lambdas.push_back(LE);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile( DL->getLocation())) return;
      if (DL->isLambda() == false)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (cur_lambdas.empty())
        return;
      auto target = cur_lambdas.back();
      if (target->getLambdaClass() != DL)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("mutable") != string::npos)
        return;
      if (content.find("const") != string::npos)
        return;
      content.insert(content.rfind('}'), "\n/*mut104*/mutable");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_104::MutatorASTConsumer_104::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}