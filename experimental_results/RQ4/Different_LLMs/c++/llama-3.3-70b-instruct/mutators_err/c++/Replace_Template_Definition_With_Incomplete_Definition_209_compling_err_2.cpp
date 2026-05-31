//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Definition_With_Incomplete_Definition_209
 */ 
class MutatorFrontendAction_209 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_209 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_209(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Template_Definition_With_Incomplete_Definition_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDefinition")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(TD->getSourceRange()), 
          *Result.SourceManager, 
          clang::LangOptions());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find('{');
      if (pos != std::string::npos) {
        declaration = declaration.substr(0, pos);
      }
      declaration += ";";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_209::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_209>(TheRewriter);
}

void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDefinition");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}