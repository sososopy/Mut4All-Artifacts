//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Attribute_Syntax_174
 */ 
class MutatorFrontendAction_174 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(174)

private:
    class MutatorASTConsumer_174 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_174(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Lambda_Attribute_Syntax_174.h"

// ========================================================================================================
#define MUT174_OUTPUT 1

void MutatorFrontendAction_174::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LambdaExpr = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaWithAttributes")) {
      //Filter nodes in header files
      if (!LambdaExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LambdaExpr->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto lambdaRange = LambdaExpr->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);

      //Perform mutation on the source code text by applying string replacement
      size_t attrPos = lambdaText.find("__attribute__");
      if (attrPos != std::string::npos) {
          size_t startPos = lambdaText.find("(", attrPos);
          size_t endPos = lambdaText.find(")", startPos);
          if (startPos != std::string::npos && endPos != std::string::npos) {
              std::string malformedAttr = "__attribute__((custom_attribute({})))";
              lambdaText.replace(attrPos, endPos - attrPos + 1, malformedAttr);
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), lambdaText);
    }
}

void MutatorFrontendAction_174::MutatorASTConsumer_174::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = lambdaExpr(hasAttr(clang::attr::AttrKind::AT_Custom)).bind("LambdaWithAttributes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}