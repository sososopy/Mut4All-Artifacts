//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Template_Parameter_Redundancy_131
 */ 
class MutatorFrontendAction_131 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_131>(TheRewriter);
    }

private:
    class MutatorASTConsumer_131 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_131(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    clang::Rewriter TheRewriter;
};

//source file
#include "../include/Inject_Template_Parameter_Redundancy_131.h"

// ========================================================================================================
#define MUT131_OUTPUT 1

void MutatorFrontendAction_131::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TemplateAlias")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(TA->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      if (TA->getTemplateParameters()) {
        auto templateParams = TA->getTemplateParameters();
        auto templateParamList = templateParams->getTemplateParameters();
        if (!templateParamList.empty()) {
          std::string newTemplateParam = ", typename U";
          size_t pos = declaration.find(">");
          if (pos != std::string::npos) {
            declaration.insert(pos, newTemplateParam);
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(TA->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_131::MutatorASTConsumer_131::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::typeAliasDecl().bind("TemplateAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}