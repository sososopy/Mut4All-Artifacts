//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Capture_Initializer_With_decltype_In_declval_Context_286
 */ 
class MutatorFrontendAction_286 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(286)

private:
    class MutatorASTConsumer_286 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_286(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Lambda_Capture_Initializer_With_decltype_In_declval_Context_286.h"

// ========================================================================================================
#define MUT286_OUTPUT 1

void MutatorFrontendAction_286::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      auto captures = MT->captures();
      bool hasInitCapture = false;
      std::string capturedVarName;
      for (const auto &capture : captures) {
        if (capture.getCaptureKind() == clang::LambdaCaptureKind::LCK_Init) {
          hasInitCapture = true;
          capturedVarName = capture.getCapturedVar()->getNameAsString();
          break;
        }
      }
      if (!hasInitCapture) return;
      auto body = MT->getBody();
      auto bodySource = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutationInsertion;
      mutationInsertion = "\n    using CapType = decltype(" + capturedVarName + ");\n";
      mutationInsertion += "    static_assert(std::is_same_v<\n";
      mutationInsertion += "        decltype(std::declval<CapType>().operator()(std::declval<decltype(param)>()))\n";
      mutationInsertion += "        , decltype(" + capturedVarName + "(param))>, \"check\");\n";
      bodySource.insert(bodySource.find("{") + 1, mutationInsertion);
      lambdaSource.replace(lambdaSource.find(bodySource), bodySource.length(), bodySource);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_286::MutatorASTConsumer_286::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAnyCapture(initCapture())).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}