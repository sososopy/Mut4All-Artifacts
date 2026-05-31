//header file
#pragma once
#include "Mutator_base.h"

/**
 * NestedStructExportMutator_290
 */ 
class MutatorFrontendAction_290 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) override;

private:
    class MutatorASTConsumer_290 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_290(clang::Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedStructExportMutator_290.h"

// ========================================================================================================
#define MUT290_OUTPUT 1

void MutatorFrontendAction_290::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
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
      std::string nestedStructName = "NestedStruct";
      std::string mutatedDeclaration = "struct " + nestedStructName + ";";
      declaration.insert(declaration.find("}"), mutatedDeclaration);
      std::string exportStatement = "export struct " + MT->getNameAsString() + "::" + nestedStructName + " {};";
      declaration.insert(declaration.find("}"), exportStatement);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_290::CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) {
    clang::Rewriter TheRewriter;
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_290>(TheRewriter);
}

void MutatorFrontendAction_290::MutatorASTConsumer_290::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}