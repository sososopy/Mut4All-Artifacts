//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Variable_With_Non_Static_Variable_448
 */ 
class MutatorFrontendAction_448 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_448 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_448(clang::Rewriter &R) : TheRewriter(R) {}
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
};

//source file
#include "../include/Replace_Static_Variable_With_Non_Static_Variable_448.h"
#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

// ========================================================================================================
#define MUT448_OUTPUT 1

void MutatorFrontendAction_448::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("StaticVariables")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string declStr = declaration.str();
      size_t pos = declStr.find("static");
      if (pos != std::string::npos) {
        declStr.erase(pos, 6);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declStr);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_448::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_448>(TheRewriter);
}

void MutatorFrontendAction_448::MutatorASTConsumer_448::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::varDecl().bind("StaticVariables");
    auto hasStaticStorageClass = clang::ast_matchers::varDecl(
        clang::ast_matchers::has(
            clang::ast_matchers::decl(
                clang::ast_matchers::hasStorageClassSpecifier(
                    clang::ast_matchers::storageClassSpecifier(
                        clang::StorageClassSpecifiers::SCS_Static
                    )
                )
            )
        )
    );
    matcher = clang::ast_matchers::anyOf(
        matcher,
        clang::ast_matchers::not(hasStaticStorageClass)
    );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}