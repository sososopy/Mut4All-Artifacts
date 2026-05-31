//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Auto_436
 */ 
class MutatorFrontendAction_436 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_436 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_436(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite, clang::ASTContext &Context, clang::SourceManager &SourceManager) 
            : Rewrite(Rewrite), Context(Context), SourceManager(SourceManager) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        clang::Rewriter &Rewrite;
        clang::ASTContext &Context;
        clang::SourceManager &SourceManager;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Template_Parameter_With_Auto_436.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParameter")) {
      //Filter nodes in header files
      if (!MT || !SourceManager.isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *SourceManager, 
          Context.getLangOpts());
      std::string str(declaration.begin(), declaration.end());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = str.find("int");
      if (pos != std::string::npos) {
        str.replace(pos, 3, "auto");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), str);
    }
}
  
void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::templateTypeParmDecl().bind("TemplateParameter");
    Callback callback(TheRewriter, Context, Context.getSourceManager());
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_436::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_436>(TheRewriter);
}