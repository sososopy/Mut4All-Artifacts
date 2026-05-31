//header file
#ifndef MUTATOR_483_H
#define MUTATOR_483_H

#include "mutator/toolings.h"
#include "mutator/utils.h"

class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
  class MutatorASTConsumer_483 : public MutatorASTConsumer {
  public:
    void HandleTranslationUnit(clang::ASTContext &Context) override;
  };

  class Callback : public MatchFinder::MatchCallback {
  public:
    Callback(clang::Rewriter &R) : Rewrite(R) {}
    void run(const MatchFinder::MatchResult &Result) override;
  private:
    clang::Rewriter &Rewrite;
  };

protected:
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
      clang::CompilerInstance &CI, clang::StringRef file) override {
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_483>(TheRewriter);
  }

private:
  clang::Rewriter TheRewriter;
};

#endif

//source file
#include "../include/Mutator_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      
      // Check if lambda is in templated context
      const clang::Decl *Parent = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc");
      if (!Parent) {
        Parent = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass");
      }
      if (!Parent) {
        return;
      }
      
      //Get the source code text of target node
      auto LambdaRange = L->getSourceRange();
      auto LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      // Find lambda introducer position
      size_t introducer_pos = LambdaText.find('[');
      if (introducer_pos == std::string::npos) {
        return;
      }
      
      // Find position after lambda parameters
      size_t param_end = LambdaText.find(']');
      if (param_end == std::string::npos) {
        return;
      }
      param_end++; // Move past ']'
      
      // Check for existing attribute
      size_t attr_pos = LambdaText.find("__attribute__");
      if (attr_pos != std::string::npos && attr_pos < LambdaText.find('{')) {
        // Lambda already has an attribute, skip mutation
        return;
      }
      
      //Choose random attribute
      int attr_index = getrandom::getRandomIndex(attribute_list.size() - 1);
      std::string attribute = "__attribute__((" + attribute_list[attr_index] + "))";
      
      //Perform mutation on the source code text by applying string replacement
      LambdaText.insert(param_end, " " + attribute);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(LambdaRange), LambdaText);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(clang::ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(
      hasAncestor(
        functionDecl(isTemplateInstantiation()).bind("TemplateFunc")
      )
    ).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}