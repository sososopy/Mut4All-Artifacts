//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_With_Parameter_Pack_489
 */ 
class MutatorFrontendAction_489 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(489)

private:
    class MutatorASTConsumer_489 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_489(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_lambda_with_parameter_pack_489.h"

// ========================================================================================================
#define MUT489_OUTPUT 1

void MutatorFrontendAction_489::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaWithPack")) {
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      auto lambdaRange = LambdaExpr->getSourceRange();
      auto lambdaCode = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

      size_t pos = lambdaCode.find("([i = Is]{}, ...);");
      if (pos != std::string::npos) {
        std::string newCode = 
          "auto handle_pack = [](auto head, auto... rest) {\n"
          "    // Do something with head\n"
          "    if constexpr (sizeof...(rest) > 0) {\n"
          "        handle_pack(rest...);\n"
          "    }\n"
          "};\n"
          "handle_pack(Is...);";
        lambdaCode.replace(pos, std::string("([i = Is]{}, ...);").length(), newCode);
        Rewrite.ReplaceText(lambdaRange, lambdaCode);
      }
    }
}
  
void MutatorFrontendAction_489::MutatorASTConsumer_489::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr(hasDescendant(packExpansionExpr())).bind("LambdaWithPack");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}