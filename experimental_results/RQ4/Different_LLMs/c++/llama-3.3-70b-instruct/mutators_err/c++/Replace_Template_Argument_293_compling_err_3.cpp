//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Template_Argument_293
 */ 
class MutatorFrontendAction_293 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(293)

private:
    class MutatorASTConsumer_293 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_293(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateSpecializationTypeLoc*> templateArgs;
    };
};

//source file
#include "../include/Replace_Template_Argument_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationTypeLoc>("TemplateSpecializationTypeLoc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTypeLoc().getBeginLoc()))
        return;
      //Get the source code text of target node
      templateArgs.push_back(MT);
      //Record the node information to be used in the mutation process
      for (const auto &arg : templateArgs) {
        if (arg != MT) {
          //Perform mutation on the source code text by applying string replacement
          std::string replacement = arg->getType().getAsString();
          std::string original = MT->getType().getAsString();
          llvm::StringRef code = clang::Lexer::getSourceText(
              CharSourceRange::getTokenRange(MT->getTypeLoc().getSourceRange()), 
              *Result.SourceManager, 
              Result.Context->getLangOpts());
          std::string mutatedCode = code.str();
          size_t pos = mutatedCode.find(original);
          if (pos != std::string::npos) {
            mutatedCode.replace(pos, original.length(), replacement);
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getTypeLoc().getSourceRange()), mutatedCode);
        }
      }
    }
}
  
void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::templateSpecializationTypeLoc().bind("TemplateSpecializationTypeLoc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}