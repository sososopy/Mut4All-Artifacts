//header file
#pragma once
#include "Mutator_base.h"
#include <sstream>

/**
 * invalid_preprocessor_directive_parameter_order_32
 */ 
class MutatorFrontendAction_32 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(32)

private:
    class MutatorASTConsumer_32 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_32(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> directives;
    };
};

//source file
#include "../include/invalid_preprocessor_directive_parameter_order_32.h"

// ========================================================================================================
#define MUT32_OUTPUT 1

void MutatorFrontendAction_32::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PP = Result.Nodes.getNodeAs<clang::PreprocessorDirective>("PreprocessorDirective")) {
      //Filter nodes in header files
      if (!PP || !Result.SourceManager->isWrittenInMainFile(
                     PP->getLocation()))
        return;

      //Get the source code text of target node
      auto directiveText = stringutils::rangetoStr(*(Result.SourceManager),
                                                   PP->getSourceRange());

      // Parse and identify parameters
      size_t start = directiveText.find(' ');
      if (start == std::string::npos) return;
      std::string directiveName = directiveText.substr(0, start);
      std::string params = directiveText.substr(start + 1);

      // Split parameters
      std::vector<std::string> paramList;
      std::istringstream iss(params);
      for (std::string s; iss >> s; )
          paramList.push_back(s);

      if (paramList.size() < 2) return;

      // Perform mutation: swap first known and unknown parameter
      std::swap(paramList[0], paramList[1]);

      // Reconstruct the directive
      std::string mutatedDirective = directiveName + " ";
      for (const auto &param : paramList) {
          mutatedDirective += param + " ";
      }
      mutatedDirective.pop_back(); // Remove trailing space

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PP->getSourceRange()), mutatedDirective);
    }
}
  
void MutatorFrontendAction_32::MutatorASTConsumer_32::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::preprocessorDirective().bind("PreprocessorDirective");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}