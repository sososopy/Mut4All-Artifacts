//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Adjust_Template_Parameter_List_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Adjust_Template_Parameter_List_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      
      //Get the source code text of target node
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = sourceText.find("...");
      if (pos != std::string::npos) {
          sourceText.insert(pos + 3, ",");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), sourceText);
    }
}
  
void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}